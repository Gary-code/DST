#include "BinTree.h"


template <class T> 
class BST:public BinTree<T>
{
public:
    virtual  BinNodeposi(T) & search(const T &);//查找
    virtual BinNodeposi(T) insert(const T & e);//插入
    virtual bool remove(const T &);//删除
protected: 
    BinNodeposi(T) _hot;//节点,最后的父亲
    BinNodeposi(T) connect34(BinNodeposi(T),BinNodeposi(T),BinNodeposi(T),
    BinNodeposi(T),BinNodeposi(T),BinNodeposi(T),BinNodeposi(T));//3+4重构
    BinNodeposi(T) rotateAt(BinNodeposi(T) );//旋转调整
};