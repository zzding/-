# 一、 0、1、2含义

**<font face="微软雅黑" size = 4>在 shell 程序中，最常使用的 fd (file descriptor) ,文件描述符大概有三个, 分别是:</font>**
<font face="微软雅黑" >
0 是一个文件描述符，表示标准输入(stdin)
1 是一个文件描述符，表示标准输出(stdout)
2 是一个文件描述符，表示标准错误(stderr)
</font>
<font face="微软雅黑" >每个进程的 0 1 2都已被占用，含义如上</font>

# 二、2 >&1
<font face="微软雅黑"  color="red"> 将标准错误重定向到标准输出</font>

<font face="微软雅黑" > 注意&表示fd，必须加上，否则含义变为将标准错误的输入到标准输出
实际的操作其实是使用dup，复制描述符，两者皆有相同的偏移量，2 >&1表示fd 2是由fd 1拷贝而来</font>

# 三、shell中的用法
<font face="微软雅黑" >./a.o>a.log 2>&1 进程a的正常输入与错误输入都会打印到log文件中</font>