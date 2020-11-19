#include "List.h"

template<class T>
Posi(T) ListNode<T>::insertAsPred(T const & e)
{
    Posi(T) x=new ListNode(e,pred,this);
    pred->succ=x;
    pred=x;
    return x;
}


template <class T>
Posi(T) ListNode<T>::insertAsSucc(T const & e)
{
    Posi(T) x=new ListNode(e,this,succ);
    succ->pred=x;
    succ=x;
}


template<class T>
void List<T>::init(){
    header=new ListNode<T>;
    trailer=new ListNode<T>;
    header->succ=trailer;
    trailer->pred=header;
    header->pred=NULL;
    trailer->succ=NULL;
    _size=0;
}

template<class T>
T List<T>::operator [](Rank r) const
{
    Posi(T) p=first();
    while (0<r--)
    {
        p=p->succ;
    }
    return p->data;
}

template<class T>
Posi(T) List<T>::find(T const & e,int n,Posi(T) p)const
{
    //从后面向前面来进行比较
    while(0<n--)
        if(e==(p=p->pred)->data) return p;
    return NULL;
}

template <class T>
Posi(T) List<T>::insertBefore(Posi(T) p,T const & e)
{
    _size++;
    return p->insertAsPred(e);
}

template <class T>
Posi(T) List<T>::insertAfter(T const & e,Posi(T) p)
{
    _size++;
    return p->insertAsSucc(e);
}

template <class T>
void List<T>::copyNodes(Posi(T) p,int n)
{
    init();
    while(n--)
    {
        insertAsLast(p->data);p=p->succ;
    }
}

template <class T>
T List<T>::remove(Posi(T) p)
{
    T e=p->data;
    p->pred->succ=p->succ;
    p->succ->pred=p->pred;
    delete p;
    _size--;
    return e;
}

template <class T>
int List<T>::clear()
{
    int oldSize=_size;
    while(0<_size)
    {
        remove(header->succ);
    }
    return oldSize;
}

template<class T>
int List<T>::deduplicate()
{
    if(_size<2)return 0;
    int oldSize=_size;
    Posi(T) p=first();
    Rank r=1;//从p的首节点开始
    while(trailer!=(p=p->succ))
    {
        Posi(T) q=find(p->data,r,p);//在p的r个前驱当中寻找
        q?remove(q):r++;
    }
    return oldSize-_size;
}

template <class T>
int List<T>::uniquify()
{
    if(_size<2) return 0;
    int oldSize=_size;
    Posi(T) p=first();
    Posi(T) q;//p为各区段的起点，q为其后继
    while(trailer!=(q=p->succ))
    {
        if(p->data!=q->data) p=q;
        else remove(q);
    }
    return oldSize-_size;
//但发现，类似于图灵机的模型，没有改进，因为call by position
}


template <class T>
Posi(T) List<T>::search(T const & e,int n,Posi(T) p)const
{
    while(0<=n--)
    {
        if(((p=p->pred)->data)<=e) break;
    }
    return p;
}