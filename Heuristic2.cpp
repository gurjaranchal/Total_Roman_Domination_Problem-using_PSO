#include "Heuristic2.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
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
int Heuristic2::FindUnlabelledIndex(double* l,int r){
    int ind =-1,c=0;
    //count unlabelled Vertex 
    for(int i=1;i<=n;i++){
        if(l[i]==-1){
            c++;
        }
    }
    //take mod of r(random value) from remaining unlabeled vertex
    r=(r%c)+1;
    c=0;
    for(int i=1;i<=n;i++){
        if(l[i]==-1){
            c++;
        }
        if(c==r)
        return i;
    }
    return ind;
}
void Heuristic2::findLabelledSet() {
    // srand(time(0)); 
    random_device rd;
    mt19937 gen(rd());
    while(FindLabelNegative(labelledSet)){
        uniform_int_distribution<> distribution(1, n);
        int r = distribution(gen);
        int currVertex = FindUnlabelledIndex(labelledSet,r);
        // cout<<endl<<"Curr Vertex "<<currVertex<<endl;
        if(labelledSet[currVertex]==-1)
        {
            vector<int> neighUnlabeled = graph.UnlabeledNeighbours(currVertex,labelledSet);
            if(neighUnlabeled.size()>=2){
                labelledSet[currVertex] =2;
                labelNeighbours(currVertex);
            }else{
                labelledSet[currVertex] =1;
                labelNeighbours(currVertex);
            }
        }
        
    }
}

double* Heuristic2::getLabelledSet(){
    return labelledSet;
}