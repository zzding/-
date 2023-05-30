```c
#define switch_to(prev, next, last)                 \
do {                                    \
    prepare_switch_to(next);                    \
                                    \
    ((last) = __switch_to_asm((prev), (next)));         \
} while (0)
```
上下文切换不仅涉及两个进程，而是三个进程。该情形如下图所示：

![](http://liujunming.top/images/2018/11/11.png)

假定3个进程A、B和C在系统上运行。在某个时间点，内核决定从进程A切换到进程B，然后从进程B切换到进程C，接下来再从进程C切换回进程A。在每个switch_to调用之前，next和prev指针位于各进程的内核栈上，prev指向当前运行的进程，而next指向将要运行的下一个进程。为执行从prev到next的切换，switch_to的前两个参数足够了。对进程A来说，prev指向进程A而next指向进程B。

在进程A被选中再次执行时，会出现一个问题，控制权返回至switch_to之后的点，如果栈准确地恢复到切换之前的状态，那么prev和next仍然指向切换之前的值，即next=B，而prev=A。在这种情况下，内核无法知道实际上在进程A之前运行的进程是C。

在新进程被选中时，底层的进程切换例程必须将此前执行的进程提供给context_switch。该宏是效果是，仿佛switch_to是带有两个参数的函数，而且返回了一个指向此前运行进程的指针。switch_to宏实际上执行的代码如下：
prev = switch_to(prev, next)
其中返回的prev值并不是用作参数的prev值，而是上一个执行的进程。在上述例子中，进程A提供给switch_to是参数是A和B，但恢复执行后得到的返回值是prev = C。内核实现该行为特性的方式依赖于底层的体系结构。