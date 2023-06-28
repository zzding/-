# 1、配置器接口
```cpp
//根据 __USE_MALLOC是否被定义决定是一级还是二级配置器
#ifdef __USE__MALLOC
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else
typedef __default_alloc_template<__NODE_ALLOCATOR_TGREADS, 0> alloc;
#endif

//配置器的接口符合STL规格
template<class T, class Alloc>
class simple_alloc {
public:
	static T* allocate(size_t n) {
		return n == 0 ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	}
	static T* allocate(void*) {
		return (T*)Alloc::allocate(sizeof(T));
	}
	static void deallocate(T* p, size_t n) {
		if (n != 0)
			Alloc::deallocate(p, n * sizeof(T));
	}
	static void deallocate(T* p) {
		Alloc::deallocate(p, sizeof(T));
	}
};

template<class T, class Alloc = alloc>
class vector {
protected:
	typedef simple_alloc<value_type, Alloc> data_allocator;
	void deallocate() {
		if () {
			data_allocator::deallocate(start, end_of_storage - start);
		}
	}
};
```