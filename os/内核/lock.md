# 互斥量的源码
```c
struct mutex {
    atomic_t count;
    spinlock_t wait_lock;
    struct list_head wait_list;
    /* ... other fields ... */
};
static inline void mutex_lock(struct mutex *lock)
{
    while (1) {
        if (atomic_dec_and_test(&lock->count))
            break;
        //用一个自旋锁来读取lock->count的状态
        spin_lock(&lock->wait_lock);
        if (!atomic_read(&lock->count)) {
            spin_unlock(&lock->wait_lock);
            break;
        }
        /* add current task to wait_list and sleep */
        spin_unlock(&lock->wait_lock);
        /* ... sleep ... */
    }
}

/* 解锁互斥锁 */
static inline void mutex_unlock(struct mutex *lock)
{
    if (atomic_inc_return(&lock->count) != 1) {
        /* wake up waiting tasks */
        spin_lock(&lock->wait_lock);
        /* ... wake up tasks ... */
        spin_unlock(&lock->wait_lock);
    }
}

```
