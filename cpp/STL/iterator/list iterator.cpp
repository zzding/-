#include <iostream>
template<typename T>
class ListItem{
public:
    T value() const{
        return value_;
    }
    ListItem* next() const{
        return next_;
    }
private:
    T value_;
    ListItem* next_;
};
template<typename T>
class List{
public:
    void insertFront(T value);
    void insertEnd(T value);
    void display(std::ostream &os = std::cout) const;
    ListItem front();
private:
    ListItem* begin_;
    ListItem* end_;
    long size_;
};


//迭代器是一种类指针，即发挥类似指针的作用，但其本身是一个class对象
//指针的自增自减操作转化为类中私有变量ptr的改变，其余的参数不变；
template <typename Item>
class ListIter{
private:
    Item* ptr;
public:
    ListIter(Item *p = 0) : ptr(p){}
    Item& operator*() const {return *ptr;}
    Item* oparator->() const {return ptr;}
    ListIter& opeator++(){
        ptr = ptr->next();
        return ptr;
    }
    ListIter oprator++(int){
        ListIter temp = *this;
        ++*this;
        return temp;
    }
    bool operator==(const ListIter& i)const{
        return ptr == i.ptr;
    }
    bool operator!=(const ListIter& i)const{
        return ptr != i.ptr;
    }

};
int main(){
    List<int> mylist;
    ListIter<ListItem<int> >begin(mylist.front());
    ListIter<ListItem<int> >end;// default 0, null   
}