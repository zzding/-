# 1、模板
```cpp
#include <iostream>
#include <vector>
template<class T, class F>
int count_arr(T begin, T end, F func) {
	int res = 0;
	for (; begin != end; ++begin) {
		if (func(*begin)) {
			res++;
		}
	}
	return res;
}
bool func1(int x) {
	if (x % 2 != 0)
		return true;
	else
		return false;
}

bool func2(int x) {
	if (x > 2)
		return true;
	else
		return false;
}
int main() {
	std::vector<int>arr = { 1, 2, 3, 4, 5 };
	std::cout << "arr odd count is " << count_arr(arr.begin(), arr.end(), func1) << std::endl;
	std::cout << "arr member big than 2 count is " << count_arr(arr.begin(), arr.end(), func2) << std::endl;
}
```

> 模板传入一个函数指针，如果希望计算数组中大于3的个数，大于5的个数，大于7的个数，难道我们要创建 许多个这样的函数吗？

```cpp
int level;
bool big(int x) {
    return x > level;
}
int main(){
    int iarr[] = { 1,12,8,13,23 };
    level = 10;
    cout << count_arr(iarr, iarr + 5, big) << endl;   
    return 0;
}
```
> 一个朴素的想法是，我搞个全局变量，需要用到的时候就修改。  
> 但是这样呢，很容易出现我们忘记去改这个全局变量。所以我们引入仿函数这个概念。

# 2、仿函数
```cpp
template <class T>
struct func {
	func(int i) : i_(i) {}
	bool operator()(T x) {
		if (x > i_) {
			return true;
		}
		else
			return false;
	}

	T i_;
};
```

# 3、lambda
```cpp
int main(){
    int iarr[] = { 1,12,8,13,23 };
    int level = 10;
    cout << count_arr(iarr, iarr + 5, [level](int x) {return x > level; }) << endl;
    return 0;
}
```