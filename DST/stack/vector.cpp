#include "vector.h"
#include <iostream>
#include <algorithm>


using namespace std;
//函数定义

template <class T>
void Vector<T>::copyFrom(T const*A,Rank lo,Rank hi){
    _elem=new T[_capacity=2*(hi-lo)];
    _size=0;//规模清零
    while(lo<hi){
        _elem[_size++]=A[lo++];
    }
} 

template <class T>
void Vector<T>::expand(){ //必要时候扩容
    if(_size<_capacity)return;//还有空余空间就不expand了
    if(_capacity<DEFAULT_CAPACITY) _capacity=DEFAULT_CAPACITY;//保证不低于最小的容量
    T * oldElem=_elem;
    _elem=new T[_capacity<<=1];
    for(int i=0;i<_size;i++)
    _elem[i]=oldElem[i];
    delete [] oldElem;//清空回收旧的
}

template <class T>
Rank Vector<T>::insert(Rank r,T const & e){//插入元素
    expand();//需要时候就去扩容
    for(int i=_size;i>r;--i){//从后向前
        _elem[i]=_elem[i-1];
    }
    _elem[r]=e;
    return r;//返回插入的秩
}

template <class T>
void Vector<T>::transverse(void(*visit) (T &)){//利用函数指针来访问
for (size_t i = 0; i < _size; i++)
{
    visit(_elem[i]);//遍历向量
}

}

template <class T> template<class VST>
void Vector<T>::transverse(VST& visit){//借助函数指针来访问
for (size_t i = 0; i < _size; i++)
{
    /* code */
    visit(_elem[i]);//遍历向量

}
}

template <class T>
int Vector<T>::disoedered()const{//返回逆序的数量
    int n=0;
    for(int i=1;i<_size;++i){
        n+=(_elem[i-1]>_elem[i]);//逆序计数
    }
    return n;//向量有序当且仅当n=0
}

template <class T>
T&  Vector<T>::operator[](Rank r)const{
    return _elem[r];//0<=r<_size 
}

template <class T>
int Vector<T>::remove(Rank lo,Rank hi){//区间删除[lo,hi)
    if(lo==hi){
        return 0;
    }
    while (hi<_size)_elem[lo++]=_elem[hi++];//顺次移动
    _size=_size-(hi-lo);shrink();//有必要的时候就缩容量
    return hi-lo;//返回被删除元素的个数
}

template <class T>
T Vector<T>::remove(Rank r){
    T e=_elem;
    remove(r,r+1);
    return e;
}

template<class T>
void Vector<T>::shrink(){
    if(_capacity<DEFAULT_CAPACITY<<1)return;// 不至于收缩到DEFAULT_CAPACITY以下
    if(_capacity<_size<<2)return;//已25%为界
    T * oldElem=_elem;
    _elem=new T[_capacity>>=1];//容量减半
    for (size_t i = 0; i < _size; i++)
    {
        _elem[i]=oldElem[i];//复制原来的内容
    }
    delete [] oldElem;//释放原来的空间
}

template<class T>
Rank Vector<T>::find(T const & e,Rank lo,Rank hi) {//无序向量的顺序查找
    while ((lo<hi--)&&(e!=_elem[hi]));//从后面开始顺序查找
    return hi;
}

template <class T>
int Vector<T>::deduplicate(){//无序向量去重
int oldSize=_size;
Rank i=1;
while(i<_size){
    (find(_elem[i],0,i)<0)?i++:remove(i);//否则删除元素
}
return oldSize-_size;//被删除元素的个数
}  

template<class T>
int Vector<T>::uniquify(){//有序向量去重复（高效）
    Rank i=0,j=0;//各对互异的秩
    while(++j<_size)
    {
        if(_elem[i]!=_elem[j])
        _elem[++i]=_elem[j];
    }
    _size=++i;
    shrink();
    return j-i;//被删除元素个数
}

template <class T>
void Vector<T>::sort(Rank lo,Rank hi){
    switch (rand()%5)
    {
    case 1:bubbleSort(lo,hi);
        break;
    case 2:selectionSort(lo,hi);
        break;
    case 3:mergeSort(lo,hi);
        break;
    case 4:heapSort(lo,hi);
        break;
    default:quickSort(lo,hi);
    
    }
}

template<class T>
Rank Vector<T>::bubble(Rank lo,Rank hi){
    Rank last=lo;
    while(++lo<hi)
    {
    if(_elem[lo-1]>_elem[lo]){
        last=lo;
        swap(_elem[lo-1],_elem[lo]);//通过交换使得局部有序
        }
    }
    return last;
}

template<class T>
void  Vector<T>::bubbleSort(Rank lo,Rank hi){
    while(lo<(hi=bubble(lo,hi)));//右边全部有序为止
}


template <class T>
void Vector<T>::merge(Rank lo,Rank mi,Rank hi){
    T* A=_elem+lo;
    int lb=mi-lo;T* B=new T[lb];
    for(Rank i=0;i<lb;++i) B[i]=A[i];
    int lc=hi-mi;T*C=_elem+mi;
    for(Rank i=0,j=0,k=0;(j<lb)||(k<lc);){
        if((j<lb)&&(!(k<lc)||(B[j]<=C[k]))) A[i++]=B[j++];
         if((k<lc)&&(!(k<lb)||(C[k]<=B[j]))) A[i++]=C[k++];
    }
    delete []B;
}

template<class T>
void Vector<T>::mergeSort(Rank lo,Rank hi){
    if(hi-lo<2)return;//单区间自然是有序的
    int mi=(lo+hi)>>1;
    mergeSort(lo,mi);
    mergeSort(mi,hi);
    merge(lo,mi,hi);//归并
}

//斐波拉契查找
template <class T>
static Rank fibSearch(T * A,T const & e,Rank lo,Rank hi){
    Fib fib(hi-lo);
    while(lo<hi){
        while(hi-lo<fib.get())fib.prev();//通过向前查找，至多迭代几代
        Rank mi=lo+fib.get()-1;
        if(e<A[mi])hi=mi;
        else if(A[mi]<e)lo=mi+1;
        else return mi;
    }
    return -1;//查找失败
}

template <class T>
static Rank binSearch(T *A,T const& e,Rank lo ,Rank hi){
    while(lo<hi){
        Rank mi=(lo+hi)>>1;
        (e<A[mi])?hi=mi:lo=mi+1;//进过比较后确定[lo,mi)或者(mi,hi)
    }
    return --lo;//lo-1为不大于e的元素的最大的秩
}


// template <class T>
// void saddleback(int A[][],int x,0,n)//马鞍查找
// {
//     int i=0;
//     int j=binSearch(A[0][],x,0,n);
//     while(i<n&&-1<j){
//         if(A[i][j]<x)i++;
//         else if(x<A[i][j])j--;
//         else
//         {
//             cout<<i<<","<<j;
//             i++;
//             j--;          
//         }
        
//     }
// }

//插值查找算法我这里就不实现了


//快速排序
template <class T>
void Vector<T>::quickSort(Rank lo,Rank hi){
    if(hi-lo<2)return ;
    Rank mi=partition(lo,hi-1);//[lo,hi]
    quickSort(lo,mi);
    quickSort(mi+1,hi);    
}
//寻找轴点
template <class T>
Rank Vector<T>::partition(Rank lo, Rank hi){//[lo,hi]
    swap(_elem[lo],_elem[lo+rand()%(hi-lo+1)]);//随机交换
    T pivot=_elem[lo];
    int mi=lo;
    for(int k=lo+1;k<=hi;++k){
        if(_elem[k]<pivot){
            swap(_elem[++mi],_elem[k])
        }
    }
    swap(_elem[lo],_elem[mi]);
}