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
    vector<double> labelledSet;

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

    vector<double> getLabelledSet(){
        return labelledSet;
    }
};

class Heuristic2 {
private:
    Graph graph;
    vector<double> labelledSet;
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

    vector<double> getLabelledSet(){
        return labelledSet;
    }
};

class particle{
    public:
    vector<double> velocity;
    vector<double> position;
    particle(int dimsize):velocity(dimsize,0),position(dimsize){}
};
class Swarm{
    private:
        Graph graph;
        Heuristic1 H1;
        Heuristic2 H2;
        vector<particle> p;
        int popsize, dim;
        int c1, c2, w,r1,r2;
        int localBest,globalBest;
    public:
        Swarm(int n,int d,const Graph& g) : graph(g),H1(graph,d),H2(graph,d){
        popsize = n;
        dim =d;
      }
        void initialise(int c1, int c2, int w){
            this->c1 = c1;
            this->c2 = c2;
            this->w = w;
        }
        void GenerateInitialSolution(){
            for(int itr = 0;itr = popsize/2; itr++){
                H1.findLabelledSet();
                p[itr].position = H1.getLabelledSet();
            }
            for(int itr = popsize/2;itr<popsize;itr++){
                H2.findLabelledSet();
                p[itr].position = H2.getLabelledSet();
            }
            
        }
        int ObjectiveValue(vector<double> p){
            int sum=0;
            for(auto it: p){
                sum+=it;
            }
            return sum;
        }
        void mainLoop(int iter){

            for(int it=0;it<iter;it++){
                int globalBestSolution;
                for(int i=0;i<popsize;i++){
                    for(int j=0;j<dim;j++){
                            //update velocity
                            p[i].velocity[j] = w * p[i].velocity[j] + c1 * r1 * (localBest - p[i].position[j]) +
                                c2 * r2 * (globalBest - p[i].position[j]);

                            //update position
                            p[i].position[j] = p[i].position[j] + p[i].velocity[j];

                            // set uper and lower bound
                            p[i].position[j] = SetThreshold(p[i].position[j]);
                        
                        }
                        
                        //check feasible
                        //make feasible
                        MakeFeasible(p[i].position);
                        localBest = ObjectiveValue(p[i].position);
                        if(localBest<globalBest){
                            globalBest = localBest;
                            globalBestSolution = i;
                        }

                    }
                }
        }

        double SetThreshold(double x){
            if(x<1.0) return 0;
            else if(x<2.0)  return 1;
            return 2;
        }
        bool HasNeighbourwithLabel1(double l,vector<double> p){
            vector<int> getNeigh = graph.neighbors(l);
            for(auto it:getNeigh){
                if(p[it]==1 || p[it]==2)return true;
            }
            return false;
        }
        bool HasNeighbourwithLabel2(double l,vector<double> p){
            vector<int> getNeigh = graph.neighbors(l);
            for(auto it:getNeigh){
                if(p[it]==2)return true;
            }
            return false;
        }
        double HasNeighbourZero(double l,vector<double> p){
            vector<int> getNeigh = graph.neighbors(l);
            for(auto it:getNeigh){
                if(p[it]==0)return it;
            }
            return -1;
        }
        void MakeFeasible(vector<double>& p){
            for(auto it:p){
                bool neigh1 = HasNeighbourwithLabel1(it,p);
                bool neigh2 = HasNeighbourwithLabel2(it,p);
                if(it==2 or it==1){
                    if(!neigh1 && !neigh2){
                        double getNeigh = HasNeighbourZero(it,p);
                        if(getNeigh!=-1) p[getNeigh] = 1;
                    }
                }else{
                    if(neigh2)continue;
                    if(!neigh1 && !neigh2){
                        double getNeigh = HasNeighbourZero(it,p);
                        if(getNeigh!=-1) p[getNeigh] =1;
                    }
                    p[it]=1;
                }
            }
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
    int popSize = 10;
    Swarm s(n,popSize,G);
    int c1=0.8, c2=0.8, w=0.5;
    s.initialise(c1,c2,w);
    s.mainLoop(100);
    return 0;
}
