#include <iostream>

using namespace std;

typedef enum{UNDISCOVERED,DISCOVERED,VISITED} VStatus;//定点的状态
typedef enum{UNDETERMINED,TREE,CROSS,FORWARD,BACKWARD} EType;//便在遍历树种所属的类型

template <class Tv,class Te>class Graph{
private:
    void reset(){
        for(int i=0;i<n;++i){
            status(i)0
        }
    }

public:
    int n;
    int e;
    

};