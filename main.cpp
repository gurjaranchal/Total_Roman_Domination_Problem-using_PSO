#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>
#include <ctime>

using namespace std;

class Graph {
public:
    unordered_map<int, vector<int>> graph;

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

    int neighborsSize(int node){
        return graph[node].size();
    }
};

class Heuristic1 {
private:
    Graph graph;
    vector<int> labelledSet;

public:
    Heuristic1(const Graph& g,int size) : graph(g), labelledSet(size+1,-1){}

    vector<int> findMaxDegreeNode(){
        vector<int> maxDegreeNodeList;
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

    void labelNeighbours(int node){
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
    void findLabelledSet() {
        srand(time(0));
        while(count(labelledSet.begin()+1,labelledSet.end(),-1)){
            vector<int> currVertexList = findMaxDegreeNode();
            int currVertex = currVertexList[(rand()%currVertexList.size())];
            labelledSet[currVertex] =2;
            labelNeighbours(currVertex);
        }
    }

    const vector<int>& getLabelledSet() const {
        return labelledSet;
    }
};

class Heuristic2 {
private:
    Graph graph;
    vector<int> labelledSet;
    int n;//total number of nodes in graph

public:
    Heuristic2(const Graph& g,int size) : graph(g), labelledSet(size+1,-1), n(size) {}

    void labelNeighbours(int node){
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
    void findLabelledSet() {
        srand(time(0)); 
        while(count(labelledSet.begin()+1,labelledSet.end(),-1)){
            int currVertex = (rand()%n) + 1;
            if(labelledSet[currVertex]==-1){
                labelledSet[currVertex] =2;
                labelNeighbours(currVertex);
            }
            
        }
    }

    const vector<int>& getLabelledSet() const {
        return labelledSet;
    }
};



int main() {
    
    Graph G;
    G.addEdge(1, 2);
    G.addEdge(1, 3);
    G.addEdge(2, 4);
    G.addEdge(3, 4);
    G.addEdge(4, 5);
    int n=5; // number of nodes

    Heuristic1 heuristicSolver1(G,n);
    heuristicSolver1.findLabelledSet();

    const auto& labelledSet1 = heuristicSolver1.getLabelledSet();
    cout << "labelled Set: ";
    for (int i=1;i<=n;i++) {
        cout << labelledSet1[i] << " ";
    }
    cout << endl;

    Heuristic2 heuristicSolver2(G,n);
    heuristicSolver2.findLabelledSet();

    const auto& labelledSet2 = heuristicSolver2.getLabelledSet();
    cout << "labelled Set: ";
    for (int i=1;i<=n;i++) {
        cout << labelledSet2[i] << " ";
    }
    cout << endl;

    return 0;
}
