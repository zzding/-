# syscall  
# 1、System call tracing

## 1、在Makefile中将$U/_trace添加到UPROGS
## 2、添加系统调用声明user/user.h   从这里来读取系统调用
## 3、usys.pl脚本中会产生用于系统调用的汇编代码
```cpp
li a7, SYS_name //SYS_name 是由syscall.h中定义的宏   a7是函数参数，具体作用于？
ecall //ecall会由用户态变为内核态
ret
```  
## 4、在proc结构体中添加一个新成员mask(参见kernel/proc.h)。初始时mask为0，表示不跟踪任何系统调用。而trace()系统调用可通过修改当前进程的mask来实现对系统调用的跟踪。

## 5、由sysproc.c中定义系统调用实现  新增一个sys_trace()，负责从寄存器中读取数据，保存在mask中

```cpp
extern uint64 sys_trace(void);
static uint64 (*syscalls[])(void)
    [SYS_trace]   sys_trace,
```
## 6、/kernel/syscall.c 会引用sysproc.c中的实现

```cpp
//根据编号来调用真正的系统调用
void syscall(void){
    // a7中保存要执行的系统调用编号
    num = p->trapframe->a7;
    //调用后的结果保存在a0
    p->trapframe->a0 = syscalls[num]();
    //跟踪系统调用打印
    if((1 << num) & p->mask) {
      printf("%d: syscall %s -> %d\n", p->pid, syscall_names[num], p->trapframe->a0);
    }
}
```

## 7、修改fork()(参见kernel/proc.c)以实现将将父进程的mask复制给子进程。