# 1、fs
```cpp
//根据路径获取inode，并写入name  调用namex
struct inode* namei(char *path)

//寻找路径的父级node，也调的namex
nameiparent(char *path, char *name) 

//根据路径名找到inode，name保存文件名称
struct inode*namex(char *path, int nameiparent, char *name)

//给定的目录（由struct inode *dp参数表示）中查找指定的文件或子目录（由char *name参数表示）。如果找到了匹配的文件或子目录，它会返回一个指向对应inode的指针。
struct inode*dirlookup(struct inode *dp, char *name, uint *poff)

//从路径中复制下一个路径元素到 name。
//返回指向复制元素后面的元素的指针。
static char* skipelem(char *path, char *name)

//在给定的目录（由struct inode *dp参数表示）中创建一个新的目录项，该目录项的名称为char *name参数，inode编号为uint inum参数
int dirlink(struct inode *dp, char *name, uint inum)

//写入inode  源inode，是否是用户空间，目的地址，偏移量，写入的大小
//真正工作的是 either_copyin(void *dst, int user_src, uint64 src, uint64 len)
//readi 同理
int writei(struct inode *ip, int user_src, uint64 src, uint off, uint n)

//复制inode信息
void stati(struct inode *ip, struct stat *st)

//截断指定的inode，即释放该inode所关联的所有数据块，并将inode的大小设置为0。
void itrunc(struct inode *ip)
//返回inode的第n块的物理地址
uint bmap(struct inode *ip, uint bn)

// 读取超级块
void readsb(int dev, struct superblock *sb)

//初始化fs 包括readsb initlog
void fsinit(int dev)

//给一个块写入0
void bzero(int dev, int bno)

//分配一个块并写入0，需要用bitmap分配
uint balloc(uint dev)

//释放一个块
void bfree(int dev, uint b)

//遍历dinode块，找到一个空闲node分配，type == 0 就是未使用的
inode* ialloc(uint dev, short type)

//更新inode到dinode
void iupdate(struct inode *ip)

//iget，它的功能是获取一个指定编号（inum）的inode。如果这个inode已经在inode缓存（icache）中，那么直接返回这个inode，并增加它的引用计数；如果这个inode不在缓存中，那么在缓存中找一个空闲的位置（即引用计数为0的位置），并将这个inode的信息（设备号和编号）保存在这个位置。

```





# 2、vm
```cpp
//user va to pa
//关键是检查flag的PTE_U是否为1，1是用户态权限
uint64 walkaddr(pagetable_t pagetable, uint64 va);

//循环使用memcopy从内核态char* src复制到用户态数据dstva，要用到walkaddr,dstva需要内存对齐到page
int copyout(pagetable_t pagetable, uint64 dstva, char *src, uint64 len);

//kernal va to pa
pte_t *walk(pagetable_t pagetable, uint64 va, int alloc)
int kcopyout(pagetable_t pagetable, uint64 dstva, char *src, uint64 len);

//父进程向子进程拷贝内存
//先拷贝物理内存，再mappages映射到子进程页表，需要拷贝flag
int uvmcopy(pagetable_t old, pagetable_t new, uint64 sz);

//解除页表和物理内存映射关系，dofree决定是否删除物理内存
// if(PTE_FLAGS(*pte) == PTE_V) 非叶子表项不可删除 好像没有提供删除一个一级或二级表项的函数
void uvmunmap(pagetable_t pagetable, uint64 va, uint64 npages, int do_free)
//递归调用freewalk释放页表，kfree删除页表占用的内存，调用函数前所有叶子表项必须全部unmap
//(pte & (PTE_R|PTE_W|PTE_X)) != 0 检查标志  
void freewalk(pagetable_t pagetable)

//解除newsz到oldsz的页表映射
//循环使用uvmunmap do_free = 1;
uint64 uvmdealloc(pagetable_t pagetable, uint64 oldsz, uint64 newsz)

//分配oldsz到newsz内存
//分配失败使用uvmdealloc释放
//使用kalloc申请内存，返回的是物理地址
//使用mappage映射关系，va是oldsz到newsz的值
uint64 uvmalloc(pagetable_t pagetable, uint64 oldsz, uint64 newsz)

//载入进程的initcode使用，申请一个物理页，建立映射va = 0的映射
//从src拷贝数据
void uvminit(pagetable_t pagetable, uchar *src, uint sz)

//给用户进程申请一个物理页，kalloc，申请的是物理地址，当作页表地址使用
pagetable_t uvmcreate()
```