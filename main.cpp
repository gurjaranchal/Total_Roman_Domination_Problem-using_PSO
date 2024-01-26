#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>
#include <ctime>
#include <climits>

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
    double* labelledSet;
    int n;

public:
    Heuristic1(const Graph& g,int size) : graph(g), n(size){
        labelledSet = new double[size+1];
        for(int i=0;i<=size;i++){
            labelledSet[i] = -1;
        }
    }

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
      bool FindLabelNegative(double* l){
        for(int i=1;i<n+1;i++){
            if(l[i]==-1){
                return true;
            }
        }
        return false;
    }
    void findLabelledSet() {
        srand(time(0));
        while(FindLabelNegative(labelledSet)){
            vector<int> currVertexList = findMaxDegreeNode();
            int currVertex = currVertexList[(rand()%currVertexList.size())];
            labelledSet[currVertex] =2;
            labelNeighbours(currVertex);
        }
    }

    double* getLabelledSet(){
        return labelledSet;
    }
};

class Heuristic2 {
private:
    Graph graph;
    double* labelledSet;
    int n;//total number of nodes in graph

public:
    Heuristic2(const Graph& g,int size) : graph(g), n(size) {
        labelledSet = new double[size+1];
        for(int i=0;i<=size;i++){
            labelledSet[i] = -1;
        }
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
    bool FindLabelNegative(double* l){
        for(int i=1;i<n+1;i++){
            if(l[i]==-1){
                return true;
            }
        }
        return false;
    }
    void findLabelledSet() {
        srand(time(0)); 
        while(FindLabelNegative(labelledSet)){
            int currVertex = (rand()%n) + 1;
            if(labelledSet[currVertex]==-1){
                labelledSet[currVertex] =2;
                labelNeighbours(currVertex);
            }
            
        }
    }

    double* getLabelledSet(){
        return labelledSet;
    }
};

class particle{
    public:
    double* velocity;
    double* position;
    void init(int d)
    {
        
        velocity = new double[d];
        position = new double[d];
    }
    // particle(int dimsize):velocity(dimsize,0),position(dimsize){}
};
class Swarm{
    private:
        Graph graph;
        Heuristic1 H1;
        Heuristic2 H2;
        int popsize, dim;
        int c1, c2, w,r1,r2;
        int localBest,globalBest;
        particle* p;
    public:
        Swarm(int n,int d,const Graph& g) : graph(g),H1(graph,d),H2(graph,d){
        popsize = n;
        dim =d;
        r1=1;
        r2=1;
        p = new particle[n];
        for(int i=0;i<n;i++){
            p[i].init(d);
        }
      }
        void initialise(int c1, int c2, int w){
            this->c1 = c1;
            this->c2 = c2;
            this->w = w;
        }
        void GenerateInitialSolution(){
            // p.resize(popsize, particle(dim));

            int mini = INT_MAX;
            for(int itr = 0;itr < popsize/2; itr++){
                H1.findLabelledSet();
                p[itr].position = H1.getLabelledSet();
                int obj = ObjectiveValue(p[itr].position);
                mini = min(mini,obj);
            }
            for(int itr = popsize/2;itr<popsize;itr++){
                H2.findLabelledSet();
                p[itr].position = H2.getLabelledSet();
                int obj = ObjectiveValue(p[itr].position);
                mini = min(mini,obj);
            }
            globalBest = mini;
            
        }
        int ObjectiveValue(double* p){
            int sum=0;
            for(int i=1;i<=dim;i++){
                sum+= static_cast<int>(p[i]);
            }
            return sum;
        }
        void mainLoop(int iter){
            for(int it=0;it<iter;it++){
                int globalBestSolution = globalBest;
                for(int i=0;i<popsize;i++){
                    localBest = i;
                    for(int j=0;j<dim;j++){
                            //update velocity
                            p[i].velocity[j] = w * p[i].velocity[j] + c1 * r1 * (p[localBest].position[j] - p[i].position[j]) +
                                c2 * r2 * (p[globalBestSolution].position[j] - p[i].position[j]);

                            //update position
                            p[i].position[j] = p[i].position[j] + p[i].velocity[j];

                            // set uper and lower bound
                            p[i].position[j] = SetThreshold(p[i].position[j]);
                        
                        }
                        
                        //check feasible
                        //make feasible
                        MakeFeasible(p[i].position);
                        cout<<"solution : ";
                        for(int k=1;k<=dim;k++){
                            cout<<p[i].position[k]<<" ";
                        }
                        cout<<endl;
                        localBest = ObjectiveValue(p[i].position);
                        if(localBest<globalBest){
                            globalBest = localBest;
                            globalBestSolution = i;
                        }

                    }
                    cout<<"gBest = "<<globalBest<<endl;
                }
                
        }
        double SetThreshold(double x){
            if(x<1.0) return 0;
            else if(x<2.0)  return 1;
            return 2;
        }
        bool HasNeighbourwithLabel1(double l,double* p){
            vector<int> getNeigh = graph.neighbors(l);
            for(auto it:getNeigh){
                if(p[it]==1 || p[it]==2)return true;
            }
            return false;
        }
        bool HasNeighbourwithLabel2(double l,double* p){
            vector<int> getNeigh = graph.neighbors(l);
            for(auto it:getNeigh){
                if(p[it]==2)return true;
            }
            return false;
        }
        int HasNeighbourZero(double l,double* p){
            vector<int> getNeigh = graph.neighbors(l);
            for(auto it:getNeigh){
                if(p[it]==0)return it;
            }
            return -1;
        }
        void MakeFeasible(double* p){
            for(int i=1;i<=dim;i++){
                bool neigh1 = HasNeighbourwithLabel1(p[i],p);
                bool neigh2 = HasNeighbourwithLabel2(p[i],p);
                if(p[i]==2 or p[i]==1){
                    if(!neigh1 && !neigh2){
                        int getNeigh = HasNeighbourZero(p[i],p);
                        if(getNeigh!=-1) p[getNeigh] = 1;
                    }
                }else{
                    if(neigh2)continue;
                    if(!neigh1 && !neigh2){
                        int getNeigh = HasNeighbourZero(p[i],p);
                        if(getNeigh!=-1) p[getNeigh] =1;
                    }
                    p[i]=1;
                }
            }
        }
};
int main() {
    
    Graph G1;
    G1.addEdge(1, 2);
    G1.addEdge(1, 3);
    G1.addEdge(2, 4);
    G1.addEdge(3, 4);
    G1.addEdge(4, 5);
    
    Graph G2;
    G1.addEdge(1, 2);
    G1.addEdge(1, 3);
    G1.addEdge(1, 5);
    G1.addEdge(3, 4);
    G1.addEdge(3, 5);
    G1.addEdge(3, 2);
    
    int n=5; // number of nodes
    int popSize = 10;
    Swarm s(popSize,n,G1);
    // Swarm s(popSize,n,G2);
    int c1=0.8, c2=0.8, w=0.5;
    s.initialise(c1,c2,w);
    s.GenerateInitialSolution();
    s.mainLoop(10);

    return 0;
}

