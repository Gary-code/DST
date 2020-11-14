#include <iostream>
#include <vector>
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
using namespace std;


template<class Tv,classs Te>class GraphMatrix:public Graph<Tv,Te>{
    private:
        vector<Vertex<Tv>> V;//顶点集
        vector<vector<Edge<Te>*>>E;//边集
     public:
        GraphMatrix(){
            
        }

        ~GraphMatrix(){

            
        }

}