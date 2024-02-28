// Heuristic2.h
#ifndef HEURISTIC2_H
#define HEURISTIC2_H
#include <bits/stdc++.h>
#include "Graph.h"
using namespace std;
class Heuristic2 {
private:
    Graph graph;
    double* labelledSet;
    int n;

public:
    Heuristic2(const Graph& g, int size): graph(g), n(size) {
        labelledSet = new double[size+1];
        for(int i=0;i<=size;i++){
            labelledSet[i] = -1;
        }
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
    int FindUnlabelledIndex(double* l,int r){
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
    void findLabelledSet() {
        // srand(time(0)); 
        random_device rd;
        mt19937 gen(rd());
        while(FindLabelNegative(labelledSet)){
            uniform_int_distribution<> distribution(1, n);
            int r = distribution(gen);
            int currVertex = FindUnlabelledIndex(labelledSet,r);
            if(labelledSet[currVertex]==-1)
            {
                // cout<<"Curr Vertex "<<currVertex<<endl;
                vector<int> neighUnlabeled = graph.UnlabeledNeighbours(currVertex,labelledSet);
                if(neighUnlabeled.size()>=2){
                    labelledSet[currVertex] =2;
                    labelNeighbours(currVertex);
                }else if(neighUnlabeled.size()==1){
                    labelledSet[currVertex] =1;
                    labelNeighbours(currVertex);
                }else{
                    if(!hasNeighbourLabel1(currVertex,labelledSet)){
                    int ind = findLabelZeroNeighbourIndex(currVertex,labelledSet);
                    labelledSet[ind] = 1;
                    }
                    labelledSet[currVertex] =1;

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
        double sum=0;
        for(int i=1;i<=n;i++){
            sum+=l[i];
            cout<<l[i]<<" ";
        }
        cout<<"cost: "<<sum<<" ";
        cout<<endl;
    }

    ~ Heuristic2(){
        delete[] labelledSet;

    }

};

#endif // HEURISTIC2_H
