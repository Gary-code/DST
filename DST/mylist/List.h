#include "stdafx.h"
#define Posi(T) ListNode<T>*


template <class T>
struct ListNode
{
    T data;
    Posi(T) pred;
    Posi(T) succ;
    ListNode(){}//针对head和trailer的构造
    ListNode(T e,Posi(T) p=NULL;Posi(T) s=NULL):data(e),pred(p),succ(s)
    {

    }

    Posi(T) insertAsPred(T const & e);//前插入
    Posi(T) insertAsSucc(T const & e);//后插入
};





template <class T>
class List
{
private:
    int _size;
    Posi(T) header;Posi(T) trailer;//头，尾哨兵节点
public:
    List(/* args */);

    ~List();
};

List::List(/* args */)
{
    
}

List::~List()
{
}
