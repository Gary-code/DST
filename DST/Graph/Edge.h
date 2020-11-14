#include <iostream>
#include <vector>
using namespace std;

typedef enum{UNDETERMINED,TREE,CROSS,FORWARD,BACKWARD} Estatus;

template<class Te> struct Edge
{
    Te data;
    int weight;//边的权重
    Estatus status;
    Edge(Te const & d,int w):data(d),weight(w),status(UNDETERMINED){}
};

