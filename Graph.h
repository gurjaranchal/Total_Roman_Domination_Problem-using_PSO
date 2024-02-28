// Graph.h
#ifndef GRAPH_H
#define GRAPH_H
#include <unordered_map>
#include "Graph.h"
#include <vector>
using namespace std;
class Graph {
public:
    unordered_map<int, std::vector<int>> graph;
    void addNode(int node) {
        if (graph.find(node) == graph.end()) {
            graph[node] = vector<int>();
        }
    }

    void addEdge(int node1, int node2) {
        addNode(node1);
        addNode(node2);

        graph[node1].push_back(node2);
        graph[node2].push_back(node1);
    }

    vector<int> nodes() {
        vector<int> result;
        for (const auto& entry : graph) {
            result.push_back(entry.first);
        }
        return result;
    }

    vector<int>& neighbors(int node) {
        return graph[node];
    }
    vector<int> UnlabeledNeighbours(int node,double* l){
        vector<int> ans;
        for(auto it:graph[node]){
            if(l[it]==-1){
                ans.push_back(it);
            }
        }
        return ans;
    }
    int neighborsSize(int node){
        return graph[node].size();
    }
    int findMaxDegree(){
        int mx=0;
        for(auto it:graph){
            vector<int> v = it.second;
            int n=v.size();
            mx=max(mx,n);
        }
        return mx;
    }
    int findMinDegree(){
        int mi=INT_MAX;
        for(auto it:graph){
            vector<int> v = it.second;
            int n=v.size();
            mi=min(mi,n);
        }
        return mi;
    }
};
#endif // GRAPH_H
