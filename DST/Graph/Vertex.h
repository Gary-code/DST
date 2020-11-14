#include <iostream>

using namespace std;

typedef enum{UNDISCOVERTED,DISCOVERED,VISITED} VStatus;//顶点的状态

template <class Tv> struct Vertex
{
    Tv data;
    int inDegree,outDegreee;//出入度数
    VStatus status;
    int dTime,fTime;//时间标签
    int parent;//遍历树中的父节点
    int priority;//遍历树当中的优先级（最短通路，极短跨边等）,优先级数
    Vertex(Tv const & d)://构造新的节点
    data(d),inDegree(0),outDegreee(0),status(UNDISCOVERTED),dTime(-1),fTime(-1),parent(-1),priority(INT32_MAX){}
    
};
