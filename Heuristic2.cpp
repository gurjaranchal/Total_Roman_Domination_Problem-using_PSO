#include "Heuristic2.h"
#include <iostream>
#include <vector>
#include <ctime>
using namespace std;

Heuristic2::Heuristic2(const Graph& g,int size) : graph(g), n(size) {
    labelledSet = new double[size+1];
    for(int i=0;i<=size;i++){
        labelledSet[i] = -1;
    }
}

void Heuristic2::labelNeighbours(int node){
    vector<int> neigh = graph.neighbors(node);
    int n=neigh.size();
    if(neigh.size()!=0){
        labelledSet[neigh[0]] = 1;
        for(int i=1; i<n;i++){
            if(labelledSet[neigh[i]]==-1)
                    labelledSet[neigh[i]]=0;
        }
    }
    
}
bool Heuristic2::FindLabelNegative(double* l){
    for(int i=1;i<n+1;i++){
        if(l[i]==-1){
            return true;
        }
    }
    return false;
}
void Heuristic2::findLabelledSet() {
    srand(time(0)); 
    while(FindLabelNegative(labelledSet)){
        int currVertex = (rand()%n) + 1;
        cout<<endl<<"Curr Vertex "<<currVertex<<endl;
        if(labelledSet[currVertex]==-1){
            labelledSet[currVertex] =2;
            labelNeighbours(currVertex);
        }
        
    }
}

double* Heuristic2::getLabelledSet(){
    return labelledSet;
}