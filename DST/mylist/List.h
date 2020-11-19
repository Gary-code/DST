#include "stdafx.h"
#define Posi(T) ListNode<T>*
typedef int Rank;
//节点结构体
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




//列表类
template <class T>
class List
{
protected:
    int _size;
    Posi(T) header;Posi(T) trailer;//头，尾哨兵节点
    void init();
    Posi(T) first(){return header->succ;}
public:
    List(int s,Posi(T) h=NULL,Posi(T) t=NULL):_size(s),h(header),trailer(t)
    {
        
    }

    List(){init();}
    //基于复制的列表构造
    List(Posi(T) p,int n){copyNodes(p,n)}

    ~List()
    {
        clear();
        delete header;
        delete trailer;
    }
    //寻位置访问，效率低，只能偶尔为之
    T operator [](Rank r)const;
    //p的n个前驱当中来进行查找
    Posi(T) find(T const & e,int n,Posi(T) p)const;
    //插入算法，前驱、后继插入
    Posi(T) insertBefore(Posi(T) p,T const & e);
    Posi(T) insertAfter(T const & e,Posi(T) p);
    Posi(T) insertAsLast(T const & e){return insertBefore(e,trailer);}
    Posi(T) insertAsFirst(T const & e){return insertAfter(e,header);}
    //列表的复制,从前向后复制
    void copyNodes(Posi(T) p,int n);
    //删除节点返回删除节点的数据
    T remove(Posi(T) p);
    //清空列表,返回原有的规模
    int clear();
    //列表的唯一化
    int deduplicate();

    //有序列表操作
    //删除重复节点,唯一化，O(n)
    int uniquify();
    //查找不大于e的最大者
    Posi(T) search(T const & e,int n,Posi(T) p)const;
};