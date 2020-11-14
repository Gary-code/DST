#include <iostream>

using namespace std;

#define BinNodeposi(T) BinNode<T>* //节点位置

template <typename T> struct BinNode{
   BinNodeposi(T) parent,lChild,rChild;
    T data;int height;
    int size();
    //constructor
		BinNode(T e,BinNodeposi(T) p=NULL,BinNodeposi(T) l=NULL,BinNodeposi(T) r=NULL,int h=0):data(e),parent(p),lChild(l),
		rChild(r),height(h){
		}
    BinNodeposi(T) insertAsLC(T const & );//作为左孩子插入
    BinNodeposi(T) insertAsRC(T const & );//作为左孩子插入
    BinNodeposi(T) succ();//中序遍历当前节点的直接后继
    //遍历
    template<class VST> void travLevel(VST &);//子树层次遍历
    //前中后序
     template<class VST> void travPre(VST &);
    template<class VST> void travIn (VST &);
    template<class VST> void travPost (VST &);
};

