问题提出

请分析此声明：void (*signal(int sig, void (*handler)(int)))(int);

求解过程

在对上面的例子作分析之前，我们需要了解C语言的声明优先级，《C专家编程》P64原文如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b26de0568d004845ae2c3cd0e34ef886.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/49bd19512d5a47c9b1320c790722e788.png)


size10：


我们使用以上规则分析void (*signal(int sig, void (*handler)(int)))(int)这个复杂的声明。从易到难的顺序：

<font size="5">**1. void (*handler)(int)：**</font><br/> 

① handler右边是圆括号，所以handler不是数组或者函数。再看其左边，是*（星号），所以handler是一个指向****的指针。

② (*handler)(int)。(*handler)的右边是圆括号，因而(*handler)(int)中的()(int)是一个返回****的函数。结合上一步分析，handler是一个指向返回****的函数的指针。


③ (*handler)(int)的右边没有符号，左边是void。综上，handler是一个指向返回void值的函数的指针。


<font size="5">**2. void (*signal(int sig, void (*handler)(int)))(int)：**</font><br/> 
void (*handler)(int)在上面的步骤已经做了分析，我们可以使用语句typedef void (*handler)(int)将其简化（将handler声明为一个新的类型，这个类型是是一个指向返回void值的函数的指针。有关typedef的知识改日再做分析），则void (*signal(int sig, void (*handler)(int)))(int)就被简化为void (*signal(int sig, handle h))(int)。下面我们对void (*signal(int sig, handle h))(int)作分析：

① signal右边是圆括号，因而signal()是一个返回****的函数。

② signal()的左边是*，因而signal()是一个返回指向****的指针的函数。

③ (*signal(int sig, handle h))右边是圆括号，因此(*signal(int sig, handle h))(int)是一个返回****的函数。综上，signal是一个返回指向返回****函数的指针的函数。

④ (*signal(int sig, handle h))右边是void。因而，signal（这个标识符）是一个返回指向返回void值的函数的指针的函数。

<font size="5">**3、int(*((*ptr(int, int))))(int);**</font><br/> 
int (**ptr(int, int))(int);
一个函数，接受两个int型的参数，返回值为一个指针A，A指向一个指针B，这个指针B指向一个参数为int型、返回值为int型的函数。

