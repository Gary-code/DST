#include "BST.h"

//查找
template <class T> 
BinNodeposi(T) & BST<T>::search(const T & e)
{
    return searchIn(_root,e,_hot=NULL); 
}

template <class T>
static BinNodeposi(T) & searchIn(BinNodeposi(T) & v,const T & e, BinNodeposi(T) & hot)//hot记忆节点
{
    //为返回失败的是后续设置了一个假象的哨兵，失败后可以返回null
    if(!v||(e==v->data))return v;
    hot=v;//记下当前访问的非空的节点
    return searchIn(((e<v->data)?v->lChild:v->rChild),e,hot);
}

template <class T> 
bool  BST<T>::remove(const T & e)
{
    BinNodeposi(T) &  x=search(e);//定位目标节点
    if(!x) return false;//确认目标节点存在
    removeAt(x,_hot);//分为两大类来删除
    _size--;
    updateHeightAbove(_hot);
    return true;
}

template <class T> static BinNodeposi(T) removeAt(BinNodeposi(T) & x,BinNodeposi(T) & hot)
{
    BinNodeposi(T) w=x;//实际被摘除的节点
    BinNodeposi(T) succ = NULL;//实际被删除节点的接替者
    if(!x->lChild) succ=x=x->rChild;
    if(!x->rChild) succ=x=x->lChild;
    else{
        //左右子树并存的情况
        w=w->succ();//中序遍历后的一个
        swap(x->data,w->data);//交换
        BinNodeposi(T) u=w->parent;
        (u==x?u->rChild:u->lChild)=succ=w->rChild;
    }
    hot=w->parent;//记录实际被删除节点的父亲
    if(succ) succ->parent=hot;
    //delete(w);
    return succ;//返回接替者
} 