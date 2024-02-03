// Graph.h
#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <vector>

class Graph {
public:
    std::unordered_map<int, std::vector<int>> graph;

    void addNode(int node);
    void addEdge(int node1, int node2);
    std::vector<int> nodes();
    std::vector<int> UnlabeledNeighbours(int node,double* l);
    std::vector<int>& neighbors(int node);
    int neighborsSize(int node);
};

#endif // GRAPH_H
