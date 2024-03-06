// Heuristic1.h
#ifndef HEURISTIC_H
#define HEURISTIC_H
#include "Graph.h"
#include <bits/stdc++.h>
using namespace std;
class Heuristic {
private:
    Graph graph;
    double* labelledSet;
    int n;

public:
    Heuristic(const Graph& g, int size): graph(g), n(size){
        labelledSet = new double[size+1];
        for(int i=0;i<=size;i++){
            labelledSet[i] = -1;
        }
    }
    vector<int> findMaxDegreeNode(double* l){
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

    void labelNeighbours(int node){
        vector<int> neigh = graph.neighbors(node);
        int n=neigh.size(),j=0;
        if(neigh.size()!=0){
            while(j<n){
                if(labelledSet[neigh[j]]==-1){
                        labelledSet[neigh[j]]=1;
                        j++;
                        break;
                    }
                    j++;
            }
            for(int i=j;i<n;i++){
                if(labelledSet[neigh[i]]==-1)
                        labelledSet[neigh[i]]=0;
            }
        }
        
    }
    bool FindLabelNegative(double* l){
        for(int i=1;i<n+1;i++){
            if(l[i]==-1){
                return true;
            }
        }
        return false;
    }
    void findLabelledSet() {
            random_device rd;
            mt19937 gen(rd());
            int t=0;
        while(FindLabelNegative(labelledSet)){
            vector<int> currVertexList = findMaxDegreeNode(labelledSet);
            uniform_int_distribution<> dis(1, currVertexList.size());
            int r= dis(gen);
            int currVertex = currVertexList[r-1];
            if(labelledSet[currVertex]==-1)
            {
                labelledSet[currVertex] =2;
                // cout<<"Curr Vertex "<<currVertex<<endl;
                vector<int> neighUnlabeled = graph.UnlabeledNeighbours(currVertex,labelledSet);
                if(neighUnlabeled.size()>=1){
                    labelNeighbours(currVertex);
                }else{
                    if(!hasNeighbourLabel1(currVertex,labelledSet)){
                    int ind = findLabelZeroNeighbourIndex(currVertex,labelledSet);
                    labelledSet[ind] = 1;
                    }
                }
            }
        }
    }
    int findLabelZeroNeighbourIndex(int v,double* l){
        vector<int> neigh = graph.neighbors(v);
        for(int i=0;i<neigh.size();i++){
            if(l[neigh[i]]==0)return neigh[i];
        }
        return -1;
    }
    bool hasNeighbourLabel1(int v,double* l){
        vector<int> neigh = graph.neighbors(v);
        for(int i=0;i<neigh.size();i++){
            if(l[neigh[i]]==1)return 1;
        }
        return 0;
    }

    double* getLabelledSet(){
        return labelledSet;
    }

    void printSolution(double* l){
        for(int i=1;i<=n;i++){
            cout<<l[i]<<" ";
        }
        cout<<endl;
    }
    ~ Heuristic(){
        delete[] labelledSet;
    }
};


#endif // HEURISTIC1_H
