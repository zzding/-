//五个用来标记用的型别
//继承的好处是可以省略函数，比如input_iterator_tag 与 forward_iterator_tag函数实现一样时，可以省略后者，
//当后者作为参数传入，会直接调用前者
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag{};
struct bindirectional_iterator_tag : public forward_iterator_tag{};
struct random_access_iterator_tag : public bindirectional_iterator_tag{};

//模板重载需要加上一个确定的能够区分函数的参数
template <class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag){
    while (n--) ++i;
}

template <class ForwardIterator, class Distance>
inline void __advance(ForwardIterator& i, Distance n, forward_iterator_tag){
    __advance(i, n, input_iterator_tag);
}

template <class BindirectionalIerator, class Distance>
inline void __advance(BindirectionalIerator& i, Distance n, bindirectional_iterator_tag){
    if(n >= 0)
        while(n--) ++i;
    else 
        while(n++) --i;
}

template <class RandomAccessIerator, class Distance>
inline void __advance(RandomAccessIerator& i, Distance n, random_access_iterator_tag){
    i += n;
}

template <class InputIterator, class Distance>
inline void advance(InputIterator &i, Distance n){
    //__advance(i, n, iterator_traits<Iterator>::iterator_category());
}

