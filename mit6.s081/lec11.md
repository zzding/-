# 1、进程切换过程
从旧进程的用户-内核转换（系统调用或中断）到内核线程，
从当前CPU的调度线程进行上下文切换，从新进程的内核线程进行上下文切换，然后从陷阱返回到用户级进程。
# 2、多CPU调度
v6调度程序每个CPU都有一个专用的线程（保存的寄存器和堆栈），因为在旧进程的内核栈上执行调度程序是不安全的：
其他核心可能唤醒该进程并运行它，在两个不同的核心上使用相同的堆栈将导致灾难。

# 3、调用过程
usertrap -------> yield ----------> sched ----------> swtch将当前的上下文保存在p->context中，并切换到之前保存在cpu->scheduler中的调度程序上下文

# 4、调度过程
- 调度器以每个CPU一个特殊的线程的形式存在，每个线程都运行调度器函数。
- 该函数负责选择下一个要运行的进程。
- 一个想要让出CPU的进程必须获取自己的进程锁p->lock，释放任何其他正在持有的锁，更新自己的状态（p->state），然后调用sched函数。
**由于持有了锁，中断应该被禁用。**
- 最后，sched函数调用swtch函数将当前上下文保存在p->context中，并切换到cpu->scheduler的调度器上下文中。
- swtch函数在调度器的堆栈上返回，就好像调度器的swtch函数返回一样。
- 调度器继续进行for循环，找到要运行的进程，切换到它，并且循环重复。

- 内核线程总是在sched函数中放弃CPU，并且总是切换到调度器中的相同位置，调度器（几乎）总是切换到之前调用sched的某个内核线程。

# 5、内核线程
内核线程：  
XV6内核共享了内存，并且XV6支持内核线程的概念，对于每个用户进程都有一个内核线程来执行来自用户进程的系统调用。  
所有的内核线程都共享了内核内存，所以XV6的内核线程的确会共享内存。
用户线程：  
每一个用户进程都有独立的内存地址空间（注，详见4.2），并且包含了一个线程，这个线程控制了用户进程代码指令的执行。  
所以XV6中的用户线程之间没有共享内存，你可以有多个用户进程，但是每个用户进程都是拥有一个线程的独立地址空间。XV6中的进程不会共享内存。  
（xv6）进程只有一个线程

# 6、运算密集型线程（compute bound thread）
即长时间占有CPU，需要时钟中断器中断。  
- 定时器中断将CPU控制权给到内核，内核再自愿的出让CPU。
实际上的做法: 定时器中断会强制的将CPU控制权从用户进程给到内核，这里是pre-emptive scheduling，  
之后内核会代表用户进程（注，实际是内核中用户进程对应的内核线程会代表用户进程出让CPU），使用voluntary scheduling。

# 7、线程第一次调度
1. 第一次调用switch时，“另一个”调用swtch函数的线程的context对象。
proc.c文件中的allocproc函数会被启动时的第一个进程和fork调用，allocproc会设置好新进程的context
```c
  // Set up new context to start executing at forkret,
  // which returns to user space.
  memset(&p->context, 0, sizeof(p->context));
  p->context.ra = (uint64)forkret;
  p->context.sp = p->kstack + PGSIZE;
```

2. 调度器调度这个线程后，会直接跳转到forkret函数
```c
// A fork child's very first scheduling by scheduler()
// will swtch to forkret.
void
forkret(void)
{
  static int first = 1;

  // Still holding p->lock from scheduler.
  release(&myproc()->lock);

  if (first) {
    // File system initialization must be run in the context of a
    // regular process (e.g., because it calls sleep), and thus cannot
    // be run from main().
    first = 0;
    fsinit(ROOTDEV);
  }

  usertrapret();
}
```
执行一些初始化操作
