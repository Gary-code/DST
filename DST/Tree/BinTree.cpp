#include "BinTree.h"
#include <iostream>
#include "Stack.h"
#include <queue>
using namespace std;


template <class T>
int BinTree<T>::updateHeight(BinNodeposi(T)x){
    return x->height=1+max(stature(x->lChild),stature(x->rChild));
}//采用常规二叉树的规则，O(1)

template <class T>
void BinTree<T>::updateHeightAbove(BinNodeposi(T) x){//全树
    while (x)//可改进，一旦高度没有改变就会
    {
        updateHeight(x);
        x=x->parent;
    }
}//O(n=depth(x))

//节点插入算法
template <class T>
BinNodeposi(T) BinTree<T>::insertAsRC(BinNodeposi(T) x,T const & e)
{
    _size++;
    x->insertAsRC(e);
    updateHeight(x);
    return x->rChild;
}

template <class T> 
BinNodeposi(T) BinTree<T>::insertAsLC(BinNodeposi(T) x,T const & e)
{
     _size++;
    x->insertAsLC(e);
    updateHeight(x);
    return x->rChild;
}

//遍历
//1.先序遍历
//递归实现,并不太好，每一帧不太小（常数不太小）
template<class T,class VST>
void traverse(BinNodeposi(T) x,VST & visit){
    if(!x)return;
    visit(x->data);
    traverse(x->lChild,visit);
    traverse(x->rChild,visit);
}//O(1)+T(a)+T(n-a-1)=O(n)

//更好的迭代实现，引入栈，对尾递归变成迭代
template <class T,class VST>
void travPre_I1(BinNodeposi(T) x,VST &visit){
    Stack <BinNodeposi(T)> S;//辅助栈
    if(x) S.push(x);
    while(!S.is_empty()){
        x=S.pop();visit(x->data);
        if(x->rChild) S.push(x->rChild);//右孩子后出去
        if(x->lChild) S.push(x->lChild);//左孩子先出去
    }
}

//因为上面这个算法不好推广到其他遍历顺序，因此要另寻捷径


template <class T,class VST>
static void visitAlongLeftBranch(BinNodeposi(T) x,VST & visit,Stack <BinNodeposi(T)>& S)
{
    while(x){
        visit(x->data);
        S.push(x->rChild);
        x=x->lChild;
    }
}

//终极版本先序遍历算法
template <class T,class VST>
void travPre_I2(BinNodeposi(T) x,VST &visit){
    Stack <BinNodeposi(T)> S;
    while(true){
        visitAlongLeftBranch(x,visit,S);
        if(S.is_empty())break;
        x=S.pop();
    }
}

//递归实现中序遍历
// template <class T,class VST>
// void traverse(BinNodeposi(T) x,VST &visit){
//     if(!x) return;
//     traverse(x->lChild,visit);//左子树
//     visit(x->data);//根节点
//     traverse(x->rChild,visit);//右子树
// }//O(n)，对常数不满意


//迭代实现中序遍历
template <class T>
static void goAlongLeftBranch(BinNodeposi(T) x,Stack <BinNodeposi(T)> & S)
{while(x) {S.push(x);x=x->lChild;}}

template<class T,class VST>
void travIn_I1(BinNodeposi(T) x,VST &visit){
    Stack <BinNodeposi(T)> S;
    while(true){
        goAlongLeftBranch(x,S);//逆序推入栈
        if(S.is_empty())break;
        x=S.pop();
        visit(x->data);    
        x=x->rChild;
    }
}

//层次遍历
template <class T> template<class VST>
void BinNode<T>:: travLevel(VST &visit){
    queue<BinNodeposi(T)> Q;
    Q.push(this);//根节点入队
    while(!Q.empty()){
        BinNodeposi(T) x=Q.pop();
        visit(x->data);
        if(x->lChild) Q.push(x-lChild);//左孩子入队
        if(x->rChild) Q.push(x->rChild);//右孩子入队
    }
}