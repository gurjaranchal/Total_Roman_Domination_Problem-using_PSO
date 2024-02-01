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

// According to max degree
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
            cout<<endl<<"Curr Vertex "<<currVertex<<endl;
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
            cout<<endl<<"Curr Vertex "<<currVertex<<endl;
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
    double* localBest;
    void init(int d)
    {
        
        velocity = new double[d];
        position = new double[d];
        localBest = new double[d];
        //initial velocity
        for(int i=0;i<d;i++){
            velocity[i] = 0; 
        }
    }
    // particle(int dimsize):velocity(dimsize,0),position(dimsize){}
};

class Swarm{
    private:
        Graph graph;
        int popsize, dim;
        double c1, c2, w,r1,r2;
        double* globalBest;
        double globalBestValue;
        particle* p;
        int minIndex;

    public:
        Swarm(int n,int d,const Graph& g) : graph(g){
        popsize = n;
        dim =d;
        globalBest = new double[d+1];
        minIndex = -1;
        p = new particle[n];
        for(int i=0;i<n;i++){
            p[i].init(d+1);
        }
      }
      
        void initialise(int c1, int c2, int w){
            this->c1 = c1;
            this->c2 = c2;
            this->w = w;
            
        }
        
        void GenerateInitialSolution(){
            
            int mini = INT_MAX;
            // apply Heuristic1 to generate Solutions
            for(int itr = 0;itr < popsize/2; itr++){
                Heuristic1 H1(graph,dim);
                cout<<"Itr "<<itr<<": ";
                H1.findLabelledSet();
                p[itr].position = H1.getLabelledSet();
                int obj = ObjectiveValue(p[itr].position);
                if(mini > obj){
                    mini = obj;
                    minIndex = itr;
                }
            }
            
            // apply Heuristic2 to generate Solutions
            for(int itr = popsize/2;itr<popsize;itr++){
                Heuristic1 H2(graph,dim);
                cout<<"Itr "<<itr<<": ";
                H2.findLabelledSet();
                p[itr].position = H2.getLabelledSet();
                int obj = ObjectiveValue(p[itr].position);
                if(mini > obj){
                    mini = obj;
                    minIndex = itr;
                }
            }
            globalBestValue = mini;
            // print initial solutions
            globalBest[0] = 0;
            cout<<"Initial Solution/Position: \n";
            for(int i=0;i<popsize;i++){
                cout<<"S"<<i<<": ";
                if(i==minIndex)globalBest = p[i].position;
                for(int k=1;k<=dim;k++){
                        cout<<p[i].position[k]<<" ";
                        p[i].localBest[k] = p[i].position[k];
                        
                }
                cout<<endl;    
            }
            cout<<endl<<"--------------------------------------------------\n"<<endl;
             cout<<"Initial Velocity: \n";
            for(int i=0;i<popsize;i++){
                cout<<"S"<<i<<": ";
                for(int k=1;k<=dim;k++){
                        p[i].velocity[k] = 0.1*p[i].position[k];
                        cout<<p[i].velocity[k]<<" ";
                }
                cout<<endl;    
            }
            cout<<endl<<"--------------------------------------------------\n"<<endl;                
        }
        int ObjectiveValue(double* p){
            int sum=0;
            for(int i=1;i<=dim;i++){
                sum+= static_cast<int>(p[i]);
            }
            return sum;
        }
        void mainLoop(int iter){
            for(int it=1;it<iter;it++){
                cout<<"Solution After "<<it<<" iteration\n"<<endl;
                for(int i=0;i<popsize;i++){
                    for(int j=1;j<=dim;j++){
                        
                            r1 = getRandomNumber(0,1);
                            r2 = getRandomNumber(0,1);
                            //update velocity
                            p[i].velocity[j] = w * p[i].velocity[j] + c1 * r1 * (p[i].localBest[j] - p[i].position[j]) +
                                c2 * r2 * (globalBest[j] - p[i].position[j]);
                            // cout<<p[i].velocity[j]<<" ";
                            //update position
                            p[i].position[j] = p[i].position[j] + p[i].velocity[j];

                            // set uper and lower bound
                            p[i].position[j] = SetThreshold(p[i].position[j]);
                        
                        }
                        
                        //check and make feasible
                        if(!CheckFeasible(p[i].position))
                           MakeFeasible(p[i].position);
                        
                        //old objective Value
                        double fitnessValueOld = ObjectiveValue(p[i].localBest);
                        // new Objective Value
                        double fitnessValueNew = ObjectiveValue(p[i].position);

                        if(fitnessValueNew < fitnessValueOld){
                            //update localBest
                            for(int k=1;k<=dim;k++){
                                p[i].localBest[k] = p[i].position[k];
                            }
                        }else {
                            //update position as previous
                            // for(int k=1;k<=dim;k++){
                            //     p[i].position[k] = p[i].localBest[k];
                            // }
                        }

                        if(globalBestValue > fitnessValueOld){
                            //update globalBest
                            for(int k=1;k<=dim;k++){
                                globalBest[k] = p[i].localBest[k];
                            }
                            globalBestValue = fitnessValueOld;
                        }
                        
                        // print updated velocity
                        cout<<"V"<<i<<": ";
                        for(int k=1;k<=dim;k++){
                            cout<<p[i].velocity[k]<<" ";
                        }
                        
                        // print feasible solution 
                        cout<<"S"<<i<<": ";
                        for(int k=1;k<=dim;k++){
                            cout<<p[i].position[k]<<" ";
                        }
                        cout<<endl;
                        // print feasible solution 
                        // cout<<"V "<<i<<": ";
                        for(int k=1;k<=dim;k++){
                            // cout<<p[i].velocity[k]<<" ";
                        }
                        cout<<endl;

                    }
                    cout<<"gBest = "<<globalBestValue<<endl;
                    cout<<endl<<"--------------------------------------------------\n"<<endl;    
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
        bool CheckFeasible(double* p){
            for(int i=1;i<=dim;i++){
                bool neigh1 = HasNeighbourwithLabel1(p[i],p);
                bool neigh2 = HasNeighbourwithLabel2(p[i],p);
                if(p[i]==1 or p[i]==2){
                   if(neigh1 || neigh2){
                      continue;
                   }else return 0; 
                }else{
                    if(!neigh2){
                       return 0; 
                    }
                }
            }
            return 1;
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
        double getRandomNumber(double lower,double upper){

    return ((double) rand()/ RAND_MAX) * (upper-lower) + lower;
}
};

int main() {
    int node,edge;
    cout<<"Enter Number of Nodes: ";
    cin>>node;
    cout<<"Enter Number of Edges: ";
    cin>>edge;
    Graph G;
    while(edge--){
        int v1,v2;
        cin>>v1>>v2;
        G.addEdge(v1,v2);

    }
    int popSize = 10;
    Swarm s(popSize,node,G);

    
    int c1=0.8, c2=0.8, w=1;
    s.initialise(c1,c2,w);
    s.GenerateInitialSolution();
    s.mainLoop(10);

    return 0;
}

/* Graph G1;
    G1.addEdge(1, 2);
    G1.addEdge(1, 3);
    G1.addEdge(2, 4);
    G1.addEdge(3, 4);
    G1.addEdge(4, 5);
    int n1=5; // number of nodes
    
    Graph G2;
    G2.addEdge(1, 2);
    G2.addEdge(1, 3);
    G2.addEdge(1, 5);
    G2.addEdge(3, 4);
    G2.addEdge(3, 5);
    G2.addEdge(3, 2);
    int n2=5; // number of nodes
    
    Graph G3;
    G3.addEdge(1,2);
    G3.addEdge(1,4);
    G3.addEdge(2,3);
    G3.addEdge(3,4);
    G3.addEdge(4,6);
    G3.addEdge(5,6);
    G3.addEdge(7,6);
    G3.addEdge(8,5);
    G3.addEdge(7,8);
    int n3=8; // number of nodes
    
    Graph G4;
    G4.addEdge(1,2);
    G4.addEdge(2,3);
    G4.addEdge(2,4);
    G4.addEdge(3,5);
    G4.addEdge(4,5);
    G4.addEdge(4,6);
    G4.addEdge(7,8);
    G4.addEdge(6,8);
    G4.addEdge(8,10);
    G4.addEdge(9,8);
    G4.addEdge(6,7);
    int n4=10;
    
    Graph G5;
    G5.addEdge(1,2);
    G5.addEdge(1,3);
    G5.addEdge(1,4);
    G5.addEdge(2,3);
    G5.addEdge(2,4);
    G5.addEdge(3,4);

    Graph G6;
    G6.addEdge(1,2);
    G6.addEdge(2,3);
    G6.addEdge(3,4);
    G6.addEdge(4,5);
    G6.addEdge(5,6);
    G6.addEdge(6,7);
    G6.addEdge(7,1);*/
