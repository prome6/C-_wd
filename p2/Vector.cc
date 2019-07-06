#include <iostream>
#include <memory>
using std::endl;
using std::cout;
using std::allocator;

template <class T>
class Vector
{
	public:
	using iterator = T *;
	using const_iterator = const T *;
		
	Vector()
		:_start(nullptr)
		 ,_finish(nullptr)
		 ,_end_of_storage(nullptr)
	{}
	~Vector();
	template<class... Args>
	void push_back(Args... args);
	void pop_back() ;
	size_t capacity() { return _end_of_storage - _start;	}
	size_t size(){	return _finish - _start; }
	T & operator [] (size_t idx) const ;
	iterator begin() { return _start; }
	iterator end() { return _finish; }
	const_iterator begin() const { return _start; }
	const_iterator end() const { return _finish; }
	private:
		void reallocate();
	private:
		static allocator<T> _alloc;
		T* _start;
		T* _finish;
		T* _end_of_storage;
};
template <class T>//静态变量初始化，不然会说未定义引用
allocator<T> Vector<T>::_alloc;

template <class T>
Vector<T>::~Vector()
{
	if(_start){
		while(_finish != _start)
			_alloc.destroy(--_finish);
	}
	if(capacity()>0)
		_alloc.deallocate(_start,capacity());
}
template <class T>
template <class... Args>
void Vector<T>::push_back(Args... args)
{
	if(capacity() == size())
	{
		reallocate();
	} 
	_alloc.construct(_finish++,args...);
}
template <class T>
void Vector<T>::reallocate()
{
	size_t capa = capacity();
	size_t newCapa = 2* capa > 0 ? 2*capa : 1;
	T* p = _alloc.allocate(newCapa);
	if(_start)
	{
		std::uninitialized_copy(_start,_finish,p);
		while(_finish != _start)
			_alloc.destroy(--_finish);
		_alloc.deallocate(_start,capa);
	}
	_start = p;
	_finish = _start + capa;
	_end_of_storage = _start + newCapa;
}

template <class T>
void Vector<T>::pop_back()
{
	if(size() > 0){
		_alloc.destroy(--_finish);
	}
}


template<class T>
T & Vector<T>::operator [](size_t idx) const 
{
	return _start[idx];
}

class Point
{
	public:
		Point(int x,int y=0)
			:_ix(x)
			 ,_iy(y)
	{}
		Point(const Point & rhs)
			:_ix(rhs._ix)
			 ,_iy(rhs._iy)
	{}
		~Point()
		{
			cout <<"~Point" << endl;
		}
		friend std::ostream & operator << (std::ostream & os,const Point & rhs);	
	private:
		int _ix,_iy;
};

std::ostream & operator << (std::ostream & os,const Point & rhs)
{
	os << "(" << rhs._ix << "," << rhs._iy << ")" ;
	return os;
}

template <class Container>
void display(const Vector<Container> & vi)
{
	typename Vector<Container>::const_iterator it = vi.begin();
	//typename 这个挺重要的用法，两层定义
	while(it != vi.end())
	{
		cout << *it <<" " ;
		++it;
	}
	cout << endl;
}


void test1()
{
	Vector<int> vi;
	vi.push_back(1);
	cout << "capacity() = " << vi.capacity() << endl;
	vi.push_back(2);
	vi.push_back(3);
	vi.pop_back();
	vi.push_back(4);
	vi.push_back(5);
	display(vi);
	cout << "capacity() = " << vi.capacity() << endl;
}

void test2()
{
	Vector<Point> points;
	points.push_back(Point(1,2));
	points.push_back(2,3);
	points.push_back(3,4);
	points.push_back(5,5);
	points.pop_back();
	points.push_back(4,5);
	points.push_back(5,6);
	display(points);
	cout << "capacity() = " << points.capacity() << endl;

}



int main()
{
//	test1();
	test2();
	return 0;
}
