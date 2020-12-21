#include "iostream"
#include "BST.h"

#define Balanced(x) (stature((x).lChild)==stature((x).rChild))

#define BalFac(x) (stature((x).lChild)-stature((x).rChild))

#define AvlBalanced(x) ((-2<BalFac(x))&&(BalFac(x)<2))


template <class T> 
class AVL:public BST<T>
{
    public:
    //需要重新平衡
        BinNodeposi(T) insert(const T &);
        bool remove (const T & e);
};