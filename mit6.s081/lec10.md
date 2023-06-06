# 1、使用锁的原则
首先，每当一个变量可以在一个CPU写入的同时另一个CPU可以读取或写入它时，应该使用锁来防止两个操作重叠。
其次，记住锁保护不变量：如果一个不变量涉及多个内存位置，通常所有这些位置都需要由一个锁来保护，以确保不变量的维护。

# 2、需要的锁
| 锁名          | 描述                                           |
|----------------|------------------------------------------------|
| bcache.lock    | 保护块缓存条目的分配                           |
| cons.lock      | 序列化对控制台硬件的访问，避免混合输出           |
| ftable.lock    | 序列化在文件表中分配struct file                 |
| icache.lock    | 保护inode缓存条目的分配                         |
| vdisk_lock     | 序列化对磁盘硬件和DMA描述符队列的访问           |
| kmem.lock      | 序列化内存的分配                               |
| log.lock       | 序列化对事务日志的操作                         |
| pipe的pi->lock  | 序列化对每个管道的操作                         |
| pid_lock       | 序列化对next_pid的递增操作                     |
| 进程的p->lock  | 序列化对进程状态的更改                         |
| tickslock      | 序列化对ticks计数器的操作                      |
| inode的ip->lock | 序列化对每个inode及其内容的操作                 |
| buf的b->lock   | 序列化对每个块缓冲区的操作                     |


# 3、锁的缺点
## 死锁
## 破坏抽象性
如果一个模块m1中方法g调用了另一个模块m2中的方法f，那么m1中的方法g需要知道m2的方法f使用了哪些锁。因为如果m2使用了一些锁，那么m1的方法g必须集合f和g中的锁，并形成一个全局的锁的排序。这意味着在m2中的锁必须对m1可见，这样m1才能以恰当的方法调用m2。
但是这样又违背了代码抽象的原则。在完美的情况下，代码抽象要求m1完全不知道m2是如何实现的。但是不幸的是，具体实现中，m2内部的锁需要泄露给m1，这样m1才能完成全局锁排序。所以当你设计一些更大的系统时，锁使得代码的模块化更加的复杂了。

# 4、关于锁的开发流程
- 先以coarse-grained lock（注，也就是大锁）开始。
- 再对程序进行测试，来看一下程序是否能使用多核。
- 如果可以的话，那么工作就结束了，你对于锁的设计足够好了；如果不可以的话，那意味着锁存在竞争，多个进程会尝试获取同一个锁，因此它们将会序列化的执行，性能也上不去，之后你就需要重构程序。


# 4、锁的实现

硬件原子操作的实现可以有很多种方法。但是基本上都是对于地址加锁，读出数据，写入新数据，然后再返回旧数据
见amoswap

# 自旋锁源代码
```c
void
initlock(struct spinlock *lk, char *name)
{
  lk->name = name;
  lk->locked = 0;
  lk->cpu = 0;
}
void
acquire(struct spinlock *lk)
{
  push_off(); // disable interrupts to avoid deadlock.
  if(holding(lk))
    panic("acquire");

  // On RISC-V, sync_lock_test_and_set turns into an atomic swap:
  //   a5 = 1
  //   s1 = &lk->locked
  //   amoswap.w.aq a5, a5, (s1)
  while(__sync_lock_test_and_set(&lk->locked, 1) != 0)
    ;

  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that the critical section's memory
  // references happen strictly after the lock is acquired.
  // On RISC-V, this emits a fence instruction.
  __sync_synchronize();

  // Record info about lock acquisition for holding() and debugging.
  lk->cpu = mycpu();
}
```

如果锁没有被持有，那么锁对象的locked字段会是0，如果locked字段等于0，我们调用test-and-set将1写入locked字段，并且返回locked字段之前的数值0。如果返回0，那么意味着没有人持有锁，循环结束。如果locked字段之前是1，那么这里的流程是，先将之前的1读出，然后写入一个新的1，但是这不会改变任何数据，因为locked之前已经是1了。
之后__sync_lock_test_and_set会返回1，表明锁之前已经被人持有了，这样的话，判断语句不成立，程序会持续循环（spin），直到锁的locked字段被设置回0。

