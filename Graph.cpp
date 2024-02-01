#include "Graph.h"
#include <vector>
using namespace std;

void Graph::addNode(int node) {
    if (graph.find(node) == graph.end()) {
        graph[node] = vector<int>();
    }
}

void Graph::addEdge(int node1, int node2) {
    addNode(node1);
    addNode(node2);

    graph[node1].push_back(node2);
    graph[node2].push_back(node1);
}

vector<int> Graph::nodes() {
    vector<int> result;
    for (const auto& entry : graph) {
        result.push_back(entry.first);
    }
    return result;
}

vector<int>& Graph::neighbors(int node) {
    return graph[node];
}

int Graph::neighborsSize(int node){
    return graph[node].size();
}


