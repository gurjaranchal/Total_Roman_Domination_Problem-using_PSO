#include "Heuristic1.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;
// namespace std;

Heuristic1::Heuristic1(const Graph& g,int size) : graph(g), n(size){
        labelledSet = new double[size+1];
        for(int i=0;i<=size;i++){
            labelledSet[i] = -1;
        }
    }

    std::vector<int> Heuristic1::findMaxDegreeNode(){
        std::vector<int> maxDegreeNodeList;
        int mx=-1;
        for(auto node:graph.nodes()){
            if(labelledSet[node]==-1){
                int s=graph.neighborsSize(node);
                if(mx<s){
                    mx = s;
                }
            }
        }
        for(auto node:graph.nodes()){
            int s=graph.neighborsSize(node);
            if(s==mx){
                maxDegreeNodeList.push_back(node);
            }
        }
        return maxDegreeNodeList;
    }

    void Heuristic1::labelNeighbours(int node){
        std::vector<int> neigh = graph.neighbors(node);
        int n=neigh.size();
        if(neigh.size()!=0){
            labelledSet[neigh[0]] = 1;
            for(int i=1; i<n;i++){
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
        srand(time(0));
        while(FindLabelNegative(labelledSet)){
            std::vector<int> currVertexList = findMaxDegreeNode();
            int currVertex = currVertexList[(rand()%currVertexList.size())];
            std::cout<<"\nCurr Vertex "<<currVertex<<"\n";
            labelledSet[currVertex] =2;
            labelNeighbours(currVertex);
        }
    }

    double* Heuristic1::getLabelledSet(){
        return labelledSet;
    }

