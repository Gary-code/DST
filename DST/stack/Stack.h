#include <iostream>
#include "vector.h"

using namespace std;


template <class T>
class Stack:public Vector<T>
{
private:
    
public:
    void push(T const & e){insert(size(),e);}
    T pop(){return remove(size()-1);}
    T & top(){return (*this)[size()-1];}//读取顶            
                                                                                                                                                    
};


