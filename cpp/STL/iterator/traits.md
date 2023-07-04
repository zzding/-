# 1、为什么会用到特性萃取
一个模板函数，他可以处理任何类型的的参数，包括POD，自定义类型，甚至是模板，
想要返回一个值，这个值需要用到参数类型，此时需要用到traits
```cpp
template <typename T>
class Iter{
 typedef T value_type;
};
template <typename T>
typename T::value_type
func(T iter){
    return *ite;
}
```

# 2、特性traits泛化
1中只能萃取自定义类型，POD类型traits需要用到偏特化，因此再抽象出一层
```cpp
template <typename T>
struct iterator_traits{
    //保留Iter中 typedef T value_type 
    typedef typename I::value_type value_type;
};

template <typename T>
struct iterator_traits<T*>{
    typedef T value_type;
}
template <typename T>
struct iterator_traits<const T*>{
    typedef T value_type;
}

template <typename T>
typename iterator_traits<T>::value_type
func(I ite){
    return *ite;
}
```

# 3、五种迭代器
```cpp
template <class I>
struct iterator_traits{
    typedef typename I::iterator_category iterator_category;
    typedef typename I::value_type value_type;
    typedef typename I::difference_type difference_type;
    typedef typename I::pointer pointer;
    typedef typename I::reference reference;
}
```

# 4、difference_type
表示两种迭代器之间的距离，不同的迭代器的距离的含义可能不同，有正有负
```cpp
template <class I, class T>
typename iterator_traits<I>::difference_type
count(I first, I last, const T& value){
   typename iterator_traits<I>::difference_type n = 0;
   for(; first != last; ++first)
        if(*first == value)
            n++;
    return 
}
```

# 5、pointer reference
```cpp
template <typename T>
struct iterator_traits<T*>{
    typedef T* pointer_type;
    typedef T& reference_type;
}
template <typename T>
struct iterator_traits<const T*>{
    typedef const T* pointer_type;
    typedef const T& reference_type;
}
```

# 6、五种迭代器
- Input Iterator: Read only
- Output Iterator: Write only
- Forward Iterator: 区间读写
- Bidirectional Iterator: 可双向移动
- Radom Access Iterator: 前三种支持oprator ++， 第四种支持operator --; 第五种支持p+n,p-n,p[n],p1-p2,p1<p2;

# 7、STL规范做法
```cpp
template <class Category,
        class T,
        class Distance = ptrdiff_t,
        class Pointer = T*,
        class Reference = T&>
struct iterator{
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
}
template <class Item>
struct ListIter:
    public iterator<Forward_iterator_tag, Item>
{}
```