/**
*
*作者:YCute
*时间:2019-11-15-10.40.56
*题目题意简单描述:用单链表实现栈
*/


#include<iostream>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<vector>
using namespace std;

/*栈数据节点*/
template<typename T>
struct Node{
    Node(T t):value(t),next(NULL){};
    Node():next(NULL){};
    T value;
    Node<T> *next;
};

template<typename T>
class Stack{
public:
    Stack();
    ~Stack();
public:
    T top();//获得栈顶元素
    T pop();//出栈
    void push(T t);//入栈
    int size();//栈大小
    bool is_empty();//判断栈空
private:
    int count;
    Node<T>* head;
};

template<typename T>
Stack<T>::Stack(){
    count=0;
    head = new Node<T>();
    head->next = nullptr;
}

template<typename T>
Stack<T>::~Stack(){
    while(head->next!=nullptr){
        Node<T> *nodeDel=head->next;
        head->next=head->next->next;
        delete nodeDel;
    }
}

/*获得栈顶元素*/
template<typename T>
T Stack<T>::top(){
    if(head->next!=nullptr){
        return head->next->value;
    }
}

/*出栈*/
template<typename T>
T Stack<T>::pop(){
    if(head->next!=nullptr){
        Node<T> *nodeDel=head->next;
        head->next=head->next->next;
        T value=nodeDel->value;
        delete nodeDel;
        --count;
        return value;
    }
}

/*入栈*/
template<typename T>
void Stack<T>::push(T t){
    Node <T> *subNode=new Node<T>(t);
    subNode->next=head->next;
    head->next=subNode;
    ++count;
}

/*获得栈元素*/
template<typename T>
int Stack<T>::size(){
    return count;
}

/*判断栈空*/
template<typename T>
bool Stack<T>::is_empty(){
    return count==0;
}
