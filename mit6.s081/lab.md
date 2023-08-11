# 1、pgtable
## 1.1给每个进程增加一个内核表副本
简述下题意：xv6的内核态只有一个直接映射到物理内存的page table，因此用户态向内核态传递信息时，需要先把虚拟地址翻译成物理地址，然后把物理地址通过寄存器传递给内核，本实验和下一个实验的目的是给每个进程创建一个kernel page table，在切换到内核态前，将SATP寄存器切换到该进程的kernel page table，就省去了“先根据自身page table得到物理地址，再将物理地址传递给内核态”的这一步。
原来所有进程共用一个内核页表
由于内核页表不包含这些映射，用户地址在内核中是无效的。因此，当内核需要使用在系统调用中传递的用户指针（例如传递给 write() 的缓冲区指针）时，内核必须首先将指针转换为物理地址。
 ```cpp
 struct proc{
    pagetable_t kpagetable;      // User kernel page table
 }
 //进程的生成和释放分别通过 `allocproc` 和 `freeproc` 两个函数实现
 //需要实现kstack分配和释放

 void user_kvmmap(pagetable_t kpagetable,uint64 va, uint64 sz,uint64 pa, int perm);
 pagetable_t user_kvmcreate();
 void kvminit();


static struct proc* allocproc(void){
  p->kpagetable = user_kvmcreate();
  char *pa = kalloc();
  if(pa == 0)
  panic("kalloc");
  uint64 va = KSTACK(0);
  user_kvmmap(p->kpagetable,va,PGSIZE,(uint64)pa,PTE_R | PTE_W);
  p->kstack = KSTACK(0);
}

//我们需要释放为内核栈所分配的物理地址，并将 `p->kstack` 置为空指针。在这里，我们调用 `user_kvmfree` 完成内核页表的释放操作：
//在进程被调度前，即在调用 `swtch(&c->context, &p->context)` 前，我们将 `satp` 置为该进程的内核页表副本，并刷新TLB，并在//返回后将该寄存器切换至总内核页表 `kernel_pagetable` 。如无进程需要被调度，我们仍将当前 `satp` 置为 `kernel_pagetable` 。

//更改kvmpa



内核态可以从进程的内核page table中，根据虚拟地址拿到物理地址，而不是通过用户态先从自身page table拿到对应的物理地址，通过寄存器将物理地址传到内核态，因此我们需要把地址映射加入到内核page table中，要注意的是要保证地址映射的地址不会超过PLIC寄存器地址。
 ```