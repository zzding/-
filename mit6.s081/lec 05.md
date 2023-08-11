# 1、trap有关寄存器
- stvec：内核在这里写入其陷阱处理程序的地址；RISC-V跳转到此处处理陷阱。
- sepc：当发生陷阱时，RISC-V将程序计数器保存在此处（因为pc随后被覆盖为stvec）。sret（从陷阱返回）指令将sepc复制到pc。内核可以写入sepc以控制sret的跳转位置。
- scause：RISC-V将一个描述陷阱原因的编号放在这里。
- sscratch：内核在此处放置一个在陷阱处理程序的开头非常有用的值。
- sstatus：sstatus中的SIE位控制是否启用设备中断。如果内核清除了SIE，RISC-V将推迟设备中断，直到内核设置SIE。SPP位指示陷阱是来自用户模式还是监管模式，并控制sret返回到哪个模式。
- satp：页表物理地址

# 2、trap流程
1. 如果陷阱是设备中断，并且sstatus的SIE位被清除，则不执行以下任何操作。
2. 通过清除SIE来禁用中断。
3. 将pc复制到sepc。
4. 在sstatus的SPP位中保存当前模式（用户模式或监管模式）。
5. 设置scause以反映陷阱的原因。
6. 将模式设置为监管模式。
7. 将stvec复制到pc。

> 注意CPU不会切换到内核的页表，也不会切换到内核的堆栈，并且除了pc之外不会保存任何其他寄存器的值。这些任务必须由内核软件执行。
  
> CPU在陷阱期间仅执行最少的工作是为了给软件提供灵活性。例如，某些操作系统在某些情况下不需要进行页表切换，这可以提高性能。



# trampoline页映射的用途：
- trampoline页里记录了trap handle的代码指令，即代码uservec，stvec寄存器包含的就是它的起始地址.
- 为什么要映射到相同位置？因为trap handle的时候会从用户页表转换到内核页表，trampoline页映射到相同位置的话，那么指令可以沿着同一位置继续运行下去。
- ecall并不会切换page table，这是ecall指令的一个非常重要的特点。所以这意味着，trap处理代码必须存在于每一个user
 page table中。因为ecall并不会切换page table，我们需要在user page table中的某个地方来执行最初的内核代码。而这个
 trampoline page，是由内核小心的映射到每一个user page table中，以使得当我们仍然在使用user page table时，内核在
 一个地方能够执行trap机制的最开始的一些指令。这里的控制是通过STVEC寄存器完成的，这是一个只能在supervisor mode下读
 写的特权寄存器。在从内核空间进入到用户空间之前，内核会设置好STVEC寄存器指向内核希望trap代码运行的位置。


 # 3、exec系统调用
 ```S
 start:
        la a0, init
        la a1, argv
        li a7, SYS_exec
        ecall
 ```
 用户代码将exec的参数放置在寄存器a0和a1中，并将系统调用号放置在a7中。系统调用号与syscalls数组中的条目相匹配，syscalls是一个函数指针表（kernel/syscall.c:108）。ecall指令将陷入内核并执行uservec、usertrap，然后执行syscall，正如我们之前所看到的。

 ```cpp
 void
syscall(void)
{
  int num;
  struct proc *p = myproc();

  num = p->trapframe->a7;
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    p->trapframe->a0 = syscalls[num]();
  } else {
    printf("%d %s: unknown sys call %d\n",
            p->pid, p->name, num);
    p->trapframe->a0 = -1;
  }
}
 ```


 # 4、页面故障
 RISC-V有三种不同类型的页面故障：
 - 加载页面故障（当加载指令无法将其虚拟地址转换），
 - 存储页面故障（当存储指令无法将其虚拟地址转换），
 - 以及指令页面故障（当指令地址无法转换）。
 scause寄存器中的值指示页面故障的类型，而stval寄存器包含无法转换的地址。

 # 5、写时复制
 1. 父进程和子进程最初共享所有物理页面，但将它们映射为只读。
 2. 因此，当子进程或父进程执行存储指令时，RISC-V CPU会引发页面故障异常。
 3. 作为对该异常的响应，内核会复制包含出错地址的页面。它将一个副本映射为子进程的读写地址空间，将另一个副本映射为父进程的读写地址空间。
 4. 在更新页表后，内核会在引发故障的进程中恢复执行引起故障的指令。
 5. 由于内核已经更新了相关的PTE以允许写入，引发故障的指令现在将无错误地执行。

 
