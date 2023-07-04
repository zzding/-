#include <cstddef>

class   random_access_iterator_tag{}; 
template <class T, class Ref, class Ptr, size_t BufSiz>
struct __deque_iterator{
    typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
    typedef __deque_iterator<T, const T&, const T*, BufSiz> const_iterator;

    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef Ptr                         pointer;
    typedef Ref                         reference;
    typedef size_t                      size_type;
    typedef ptrdiff_t                   difference_type;
    typedef T**                         map_pointer;
    typedef __deque_iterator            self;

    T*  cur;
    T*  first;
    T*  last;
    map_pointer node;   

    static size_t buffer_size(){return __deque_buf_size(BufSiz, sizeof(T))};
    inline size_t __deque_buf_size(size_t n, size_t sz){
        return n != 0 ? n : (sz < 51 ? size_t(512 / sz)) : size_t(1);
    }

    void set_node(map_pointer new_node){
        node = new_node;
        first = *node;
        last = first + difference_type(buffer_size());
    }
    reference operator*() const {return *cur;}
    pointer operator->() const {return &(operator*());}
    self& operator++(){
        ++cur;
        if(cur == last){
            set_node(node + 1);
            cur = first;
        }
        return *this;
    }
    self operator++(int){
        self temp = *this;
        ++*this;
        return temp;
    }
    self& operator--(){
        if(cur == first){
            set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }
    self operator--(int){
        self temp = *this;
        --*this;
        return temp;
    }
    self& operator+=(difference_type n){
        difference_type offset = n + (cur - first);
        if(offset >= 0 && offset < difference_type(buffer_size()));
            cur += n;
        else {
            difference_type node_offset = 
                offset > 0 ? offset / difference_type(buffer_size())
                    : -difference_type((-offset - 1) / buffer_size()) -1;
            set_node(node + node_offset);
            cur = first + (offset - node_offset * difference_type(buffer_size()));
        }
        return *this;
    }
};