#include <iostream>
#include "BinNode.h"

using namespace std;

template <class T> BinNodeposi(T) BinNode<T>::insertAsLC(T const & e)
{
    return lChild=new BinNode(e);
}

template <class T> BinNodeposi(T) BinNode<T>::insertAsRC(T const & e)
{
    return rChild=new BinNode(e);
}

template<class T>
int BinNode<T>::size()//后代总数
{
    int s=1;//计入本身
    //递归
    if(lChild) s+=lChild->size();
    if(rChild) s+=rChild->size();
    return s;
}//O(n=|size|),遍历

