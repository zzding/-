# 1、extern 变量
extern导入的变量必须是全局变量，不能是静态变量；  
使用时加extern，定义时不加；  
使用extern时，会忽略掉本文件中的相同名字的全局变量，使用外部的值；



# 2、extern 函数
函数默认是extern的，不用加
在c++ 中，使用外部全局函数需要在本文件中声明一下。  测试中c貌似不用
```c
#include <stdio.h>
int func(){
        printf("hello\n");
}

```

```c
int main(){
        func();
}

```
编译器会警告，但是仍然可以通过编译



# 3、extern "C"

> 作为一种面向对象的语言， C++ 支持函数重载，而过程式语言 C 则不支持。所以，函数被 C++ 编译后在符号库中的名字与 C 语言的有所不同
```cpp
void foo( int x, int y );
```
> 该函数被 C 编译器编译后在符号库中的名字为 _foo ，而 C++ 编译器则会产生像 _foo_int_int 之类的名字（不同的编译器可能
生成的名字不同，但是都采用了相同的机制，生成的新名字称为 mangled name ）。 _foo_int_int 这样的名字包含了函数名、函数参
数数量及类型信息， C++ 就是靠这种机制来实现函数重载的  
> 同样地， C++ 中的变量除支持局部变量外，还支持类成员变量和全局变量。用户所编写程序的类成员变量可能与全局变量同名，我们
以 . 来区分。而本质上，编译器在进行编译时，与函数的处理相似，也为类中的变量取了一个独一无二的名字，这个名字与用户程序中
同名的全局变量名字不同。


# 4、c引用c++
> 在C中引用 C++ 语言中的函数和变量时， C++ 的头文件需添加 extern "C" ，但是在 C 语言中不能直接引用声明了 extern "C"
 的该头文件(因为C语言不支持 extern "C" 关键字，所以会报编译错误)，应该仅在 C 文件中用 extern 声明 C++ 中定义的 
 extern "C" 函数(就是 C++ 中用 extern "C" 声明的函数，**在 C 中用 extern 来声明一下**，这样 C 就能引用 C++ 的函数了，但是 C 中是不用用 extern "C" 的)。

 ```cpp
 //C++头文件 cppExample.h
#ifndef CPP_EXAMPLE_H
#define CPP_EXAMPLE_H
extern "C" int add( int x, int y );
#endif

//C++实现文件 cppExample.cpp
#include "cppExample.h"
int add( int x, int y )
{
    return x + y;
}

/* C实现文件 cFile.c
/* 这样会编译出错：#include "cExample.h" */
extern int add( int x, int y );

int main( int argc, char* argv[] )
{
    add( 2, 3 );   
    return 0;
}
```
 
> 上面例子， C 实现文件 cFile.c 不能直接用 #include "cExample.h"= 因为 =C 语言不支持 extern "C" 关键字。这个时候，
而在 cppExample.h 中使用 extern "C" 修饰的目的是为了让 C++ 编译时候能够生成 C 形式的符号(类似 _foo 不含参数的形式)，
然后将其添加到对应的 C++ 实现库中，以便被 C 程序链接到。