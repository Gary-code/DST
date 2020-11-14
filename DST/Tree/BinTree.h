#include "BinNode.h"
#include <iostream>

using namespace std;

template <class T>
class BinTree
{
protected:
    int _size;//规模
    BinNodeposi(T) _root;//根节点
    virtual int updateHeight(BinNodeposi(T) x);//更新节点x的高度
    void updateHeightAbove(BinNodeposi(T) x);//更新祖先的高度

public:
    int size()const{return _size;}
    bool empty()const{return !_root;}//判空
    BinNodeposi(T) root()const{return _root;};//返回树根
    BinNodeposi(T) insertAsRC(BinNodeposi(T) x,T const & e);
    BinNodeposi(T) insertAsLC(BinNodeposi(T) x,T const & e);
};