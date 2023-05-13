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