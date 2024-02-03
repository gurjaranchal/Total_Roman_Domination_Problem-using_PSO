#include "Heuristic1.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include<random>
using namespace std;
// namespace std;

Heuristic1::Heuristic1(const Graph& g,int size) : graph(g), n(size){
    labelledSet = new double[size+1];
    for(int i=0;i<=size;i++){
        labelledSet[i] = -1;
    }
}

vector<int> Heuristic1::findMaxDegreeNode(double* l){
    vector<int> maxDegreeNodeList;
    int mx=-1;
    for(int i=1;i<=n;i++){
        if(l[i]==-1){
            vector<int> v=graph.UnlabeledNeighbours(i,l);
            int s=v.size();
            if(mx<s){
                mx = s;
            }
        }
    }
   for(int i=1;i<=n;i++){
        if(l[i]==-1){
            vector<int> v=graph.UnlabeledNeighbours(i,l);
            int s=v.size();
            if(mx==s){
               maxDegreeNodeList.push_back(i);
            }
        }
    }
    
    return maxDegreeNodeList;
}

void Heuristic1::labelNeighbours(int node){
    vector<int> neigh = graph.neighbors(node);
    int n=neigh.size();
    if(neigh.size()!=0){
        labelledSet[neigh[0]] = 1;
        for(int i=1; i<neigh.size();i++){
            if(labelledSet[neigh[i]]==-1)
                    labelledSet[neigh[i]]=0;
        }
    }
    
}
bool Heuristic1::FindLabelNegative(double* l){
    for(int i=1;i<n+1;i++){
        if(l[i]==-1){
            return true;
        }
    }
    return false;
}
void Heuristic1::findLabelledSet() {
        random_device rd;
        mt19937 gen(rd());
        int t=0;
    while(FindLabelNegative(labelledSet)){
        vector<int> currVertexList = findMaxDegreeNode(labelledSet);
        uniform_int_distribution<> distribution(1, currVertexList.size());
        int r= distribution(gen);
        int currVertex = currVertexList[r-1];
        if(labelledSet[currVertex]==-1)
        {
        labelledSet[currVertex] =2;
        labelNeighbours(currVertex);
        }
    }
}

double* Heuristic1::getLabelledSet(){
    return labelledSet;
}

