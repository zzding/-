# 1、xv6文件系统
## 难点：
- 文件系统需要在磁盘上使用数据结构来表示命名目录和文件的树形结构，记录每个文件内容所在的块的标识，并记录磁盘上哪些区域是空闲的。
- 文件系统必须支持崩溃恢复。也就是说，如果发生崩溃（例如电源故障），文件系统必须在重新启动后正确工作。  
风险在于崩溃可能中断一系列更新操作，并导致不一致的磁盘上的数据结构（例如，一个既被文件使用又被标记为空闲的块）。
- 不同的进程可能同时对文件系统进行操作，因此文件系统代码必须协调工作以保持不变性。
- 访问磁盘的速度比访问内存慢几个数量级，因此文件系统必须维护一个内存中的热门块缓存。


# 2、文件层次
|      层次        |    描述    |    
|-----------------|-------------|
| File Descriptor |用文件系统接口来抽象许多Unix资源（例如管道、设备、文件等），简化应用程序员的工作|
|   Pathname      |提供类似于"/usr/rtm/xv6/fs.c"的层次化路径名，并通过递归查找解析它们|
|   Directory     |一系列目录项，每个目录项包含文件的名称和i节点号|
|     Inode       |层提供单个文件，每个文件由一个唯一的i节点和一些保存文件数据的块组成|
|    Logging      |允许较高层将对多个块的更新封装为事务，并确保在崩溃的情况下以原子方式更新这些块|
| Buffer Cache    |缓存层缓存磁盘块并同步对它们的访问，确保一次只有一个内核进程可以修改任何特定块中存储的数据|
|      Disk       |读取和写入virtio硬盘上的块|


# 3、磁盘分布
文件系统不使用块0（它保存引导扇区）。  
块1称为超级块，它包含关于文件系统的元数据（文件系统大小（以块为单位）、数据块数量、i节点数量和日志中的块数量）。  
从块2开始是日志块。日志之后是i节点，每个块包含多个i节点。  
之后是跟踪使用的数据块的位图块。  
剩下的块是数据块；每个数据块在位图块中标记为可用或者保存文件或目录的内容。超级块由一个名为mkfs的单独程序填充，该程序构建初始文件系统。  
| boot | super | log | inodes | bit map | data .... data |
|------|-------|-----|--------|---------|----------------|
|0|1|2|


# 4、缓存区
- 同步访问磁盘块，确保内存中只有一个块的副本，并且同一时间只有一个内核线程使用该副本
- 缓存常用的块，以避免需要从缓慢的磁盘重新读取。相关代码位于bio.c文件中

## 字段
- valid字段表示缓冲区是否包含块的副本。
- disk字段表示缓冲区的内容已经交给磁盘，磁盘可能会改变缓冲区的内容

## bread，bwrite
一旦bread从磁盘读取（如果需要）并将缓冲区返回给调用者，调用者就独占了该缓冲区，并可以读取或写入数据字节。如果调用者修改了缓冲区，必须在释放缓冲区之前调用bwrite将更改后的数据写入磁盘。


# 5、inode
```c
struct dinode {
  short type;           // File type
  short major;          // Major device number (T_DEVICE only)
  short minor;          // Minor device number (T_DEVICE only)
  short nlink;          // Number of links to inode in file system
  uint size;            // Size of file (bytes)
  uint addrs[NDIRECT+1];   // Data block addresses
};
```

类型字段区分文件、目录和特殊文件（设备）  类型为零表示磁盘上的inode为空闲。  
nlink字段计算引用此inode的目录项数目  
size字段记录文件中内容的字节数  
addrs数组记录持有文件内容的磁盘块的块编号  



# 6、sectors block
> sector通常是磁盘驱动可以读写的最小单元，它过去通常是512字节。  
> block通常是操作系统或者文件系统视角的数据。它由文件系统定义，在XV6中它是1024字节。所以XV6中一个block对应两个sector。通常来说一个block对应了一个或者多个sector。


# 7、direct block number 与 indirect block number

> XV6的inode中总共有12个block编号。这些被称为direct block number。这12个block编号指向了构成文件的前12个block。举个例子，如果文件只有2个字节，那么只会有一个block编号0，它包含的数字是磁盘上文件前2个字节的block的位置。  
> 之后还有一个indirect block number，它对应了磁盘上一个block，这个block包含了256个block number，这256个block number包含了文件的数据。所以inode中block number 0到block number 11都是direct block number，而block number 12保存的indirect block number指向了另一个block。


# 8、block编号与能支持的磁盘大小
一个int类型占4字节，因此一个block(1kb)可以储存256个block编号，4字节为32位，因此支持的磁盘大小为$2^32$个block，也就是4TB

# 9、目录
每一个目录包含了directory entries，每一条entry都有固定的格式：
- 前2个字节包含了目录中文件或者子目录的inode编号，
- 接下来的14个字节包含了文件或者子目录名。  
一条目录共16个字节

# 10、根据路径索引文件过程
## 如/y/x
先从root inode 开始，这个inode有固定存取位置，在direct block和indirect block中搜索y这个文件，  
索引到后拿到y的block inode编号，跳转到这个inode 以此类推

# 11、创建文件过程
动作：block number
1. write：33   //寻找空闲的inode，改变inode的type
2. write：33   //实际写入inode的东西，比如nlink = 1等
3. write：46   //在根目录下创建文件，因此需要增加一条目录
4. write：32   //改变根目录大小
5. write：33

# 12、写入文件的过程
1. write：45  //更新bitmap，找到一个未被使用的block
2. write：595 //写入几个字符调用几次
3. write：595
4. write：33  //inode更新size
