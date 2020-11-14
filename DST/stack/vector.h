#include <iostream>

using namespace std;

typedef int Rank;//秩

#define DEFAULT_CAPACITY 3//设置默认容量

template <class T> class Vector   //向量类模板
{
	protected:
		Rank _size;//规模
		int _capacity;//容量
		T* _elem;//数据区
		
		//复制[lo,hi)
		void copyFrom(T const * A,Rank lo,Rank hi);
		//扩容
		void expand();
		//装填因子过小时候压缩
		void shrink();
		//扫描交换
		Rank bubble(Rank lo,Rank hi);
		//冒泡排序
		void bubbleSort(Rank lo,Rank hi);
		//选取最大的元素
		Rank max(Rank lo,Rank hi);
		//选择排序
		void selectionSort(Rank lo,Rank hi);
		//归并算法
		void merge(Rank lo,Rank mid,Rank hi);
		//归并排序
		void mergeSort(Rank lo,Rank hi);
		//轴点构造法，快速排序当中轴点的构造
		Rank partition(Rank lo, Rank hi);
		//快速排序
		void quickSort(Rank lo,Rank hi);
		//堆排序
		void heapSort(Rank lo,Rank hi);

	public:

		//构造函数

        //容量为c，规模为s，所有元素初始化为v
		Vector(int c=DEFAULT_CAPACITY,int s=0,T v=0){
			_elem=new T[_capacity=c];
			for (_size=0;_size<s;_elem[_size++]=v);
        }

		//数组整体复制
		Vector(T const* A,Rank n){
		copyFrom(A,0,n);
		}

		//数组区间复制
		Vector(T const *A,Rank lo,Rank hi){
			copyFrom(A,lo,hi);
		}

		//向量整体复制
		Vector(Vector<T> const & V){
			copyFrom(V._elem,0,V._size);
		}

		//向量区间复制
		Vector(Vector<T> const & V,Rank lo,Rank hi){
			copyFrom(V._elem,lo,hi);
		}
		 
		 //析构函数
		 ~Vector(){delete [] _elem;}

		 //只读取访问的接口
		 Rank size() const{return _size;}//规模
		 bool empty()const {return !_size;}//判断向量是否为空
		 int disoedered()const;//判断向量是否排好序了
		 Rank find(T const & e)const{return find(e,0,_size);}//无序向量查找
		 Rank find(T const & e,Rank lo,Rank hi);//有序向量的查找
		 Rank search(T const& e)const{//有序向量的搜索
		 return (0>=_size) ? -1:search(e,0,_size);
		 }
		 Rank search(T const & e,Rank lo,Rank hi);//有序向量的查找
		 
		 //可写访问接口
		 T & operator [](Rank r)const;//重载下标运算符
		 Vector<T> & operator= (Vector<T> const&);//重载赋值运算符
		 T remove(Rank r);//删除秩为r的元素
		 int remove(Rank lo,Rank hi);//删除[lo,hi)之间的元素
		 Rank insert(Rank r,T const& e);//插入元素
		 Rank insert(T const& e){return insert(_size,e);}//默认作为末尾元素插入
		 void unsort(Rank lo,Rank hi);//有序置乱
		 void unsort(){unsort(0,_size);}//整体置乱
		 int deduplicate();//无序去重
		 int uniquify();//有序去重
		 void sort(Rank lo,Rank hi);//排序
		 void sort(){sort(0,_size);}//默认整体排序
		 //遍历
		 void transverse(void(*visit)(T &));//使用函数指针，只读或者局部性修改
		 template <class VST> void transverse(VST &);//使用函数对象进行遍历，可以进行全局性修改
 };

