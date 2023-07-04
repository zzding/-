#include <algorithm>
class alloc{};
//class iterator{};
template<class T, class Alloc>
class simplle_alloc {
    void allocate(size_t len);
    void deallocate(iterator i, size_t n);
};
class iterator{};
template <class T>
void construct(iterator pos,T x);
void destroy(iterator begin, iterator end);
void destroy(iterator pos);
template <class T, class Alloc = alloc>
class TinyVector{
public:
    typedef T           value_type;
    typedef T*          iterator;
    typedef T*          pointer;
    typedef T&          reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;

protected:
    typedef     simplle_alloc<value_type, Alloc>    data_allocator;
    iterator    start;
    iterator    end;
    iterator    end_of_storage;

public:
    iterator    begin()     {return start;}
    iterator    end()       {return end;}
    size_type   size()      {return size_type(end() - begin());}
    size_type   capacity()  {return size_type(end_of_storage - begin());}
    bool        empty()     {return begin() == end();}
    reference   front()     {return *begin();}
    reference   back()      {return *end();}
    reference   operator[](){return *(begin() + n);}

public:
    void insert_aux(iterator pos, value_type x);
    void deallocate(){
        if (start){
            data_allocator::deallocate(begin(), capacity());
        }
    }
    void push_back(const value_type& x){
        if(end != end_of_storage){
            construct(end, x);
            ++end;
        }else{
            insert_aux(end_of_storage, x);
        }
    }
    void pop_back(){
        if(empty()) return;
        destroy(end);
        --end;
    }
    iterator erase(iterator position){
        if(position + 1 != end())
            std::copy(position + 1, end, position);
        --end;
        destroy(end);
        return position;
    }

};


template <class T, class Alloc>
void TinyVector<T, Alloc>::insert_aux(iterator pos, value_type x){
    size_t oldsize = size();
    size_t newsize = oldsize == 0 ? 1 : 2 * oldsize;
        iterator newbegin = data_allocator::allocate(newsize);
        iterator newend = newbegin;
    try{
        newend = uninitilalisze_copy(start, pos, newbegin);
        construct(newend, x);
        ++newend;
        newend = uninitilalisze_copy(pos, end, newend); 
    }catch(...){
        destroy(newbegin, newend);
        data_allocator::deallocate(newbegin, newsize);
        throw;
    }
    start = newbegin;
    end = newend;
    end_of_storage = newbegin + newsize;
    
}