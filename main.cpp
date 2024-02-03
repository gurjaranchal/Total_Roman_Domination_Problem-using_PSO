#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>
#include <ctime>
#include <climits>
#include "Graph.h"
#include "Heuristic1.h"
#include "Heuristic2.h"
// #include "Debug.h"

using namespace std;

class particle{
    public:
    double* velocity;
    double* position;
    double* localBest;
    double* localBestV;
    void init(int d)
    {
        
        velocity = new double[d];
        position = new double[d];
        localBest = new double[d];
        localBestV = new double[d];
        //initial velocity
        // for(int i=0;i<d;i++){
        //     velocity[i] = 0; 
        // }
    }
};

class Swarm{
    private:
        Graph graph;
        int popsize, dim;
        double c1, c2, w;
        double* globalBestSolution;
        double globalBestValue;
        particle* p;
        int minIndex;

    public:
        Swarm(int n,int d,const Graph& g) : graph(g){
        popsize = n;
        dim =d;
        globalBestSolution = new double[d+1];
        minIndex = -1;
        p = new particle[n];
        for(int i=0;i<n;i++){
            p[i].init(d+1);
        }
      }
      
        void initialise(double c1, double c2, double w){
            this->c1 = c1;
            this->c2 = c2;
            this->w = w;
            
        }
        
        void GenerateInitialSolution(){
            
            int minimumObjective = INT_MAX;
            // apply Heuristic1 to generate Solutions
            for(int itr = 0;itr < popsize/2; itr++){
                Heuristic1 H1(graph,dim);
                H1.findLabelledSet();
                double* l = H1.getLabelledSet();
                copy(l, l + dim + 1, p[itr].position);
                int obj = ObjectiveValue(p[itr].position);
                if(minimumObjective > obj){
                    minimumObjective = obj;
                    minIndex = itr;
                }
            }
            
            // apply Heuristic2 to generate Solutions
            for(int itr = popsize/2;itr<popsize;itr++){
                Heuristic2 H2(graph,dim);
                H2.findLabelledSet();
                double* l = H2.getLabelledSet();
                copy(l, l + dim + 1, p[itr].position);
                int obj = ObjectiveValue(p[itr].position);
                if(minimumObjective > obj){
                    minimumObjective = obj;
                    minIndex = itr;
                }
            }
            globalBestValue = minimumObjective;
            // // print initial solutions and update local best and global best solutions
            // cout<<"Initial Solution/Position: \n";
            for(int i=0;i<popsize;i++){
                // cout<<"S"<<i<<": ";
                if(i==minIndex)globalBestSolution = p[i].position;
                for(int k=1;k<=dim;k++){
                        // cout<<p[i].position[k]<<" ";
                        p[i].localBest[k] = p[i].position[k];
                }
                // cout<<endl;    
            }
            // cout<<endl<<"--------------------------------------------------\n"<<endl;
            //initilaize the velocity
            // cout<<"Initial Velocity: \n";
            for(int i=0;i<popsize;i++){
                for(int k=1;k<=dim;k++) {
                p[i].velocity[k] = 0.1*p[i].position[k];
                p[i].localBestV[k] = p[i].velocity[k];
                }
            }
            // print velocity
            // for(int i=0;i<popsize;i++){
            //     // cout<<"\nV"<<i<<": ";
            //     for(int k=1;k<=dim;k++)cout<<p[i].velocity[k]<<" "; 
            // }
            // cout<<endl<<"--------------------------------------------------\n"<<endl;  
            // cout<<"After Initial Solution Gbest Solution is: ";
            // for(int i=0;i<=dim;i++)cout<<globalBestSolution[i]<<" ";    
            // cout<<endl<<"--------------------------------------------------\n"<<endl;            
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
                // cout<<"Solution After "<<it<<" iteration\n"<<endl;
                for(int i=0;i<popsize;i++){
                    for(int j=1;j<=dim;j++){
                            // random value in between [0,1] 
                            // use if any custom range ((double) rand()/ RAND_MAX) * (upper-lower) + lower
                            double r1 = ((double)rand() / (RAND_MAX));
                            double r2 = ((double)rand() / (RAND_MAX));
                            // cout<<endl<<"r1: " <<r1<<", r2: "<<r2<<endl;
                            //update velocity
                            double a=(w * p[i].velocity[j]) + (c1 * r1 * (p[i].localBest[j] - p[i].position[j])) +
                                (c2 * r2 * (globalBestSolution[j] - p[i].position[j]));
                            p[i].velocity[j] = a;
                            // cout<<a<<" , "<<p[i].velocity[j]<<" ";
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
                        // cout<<"Fitness Old: "<<fitnessValueOld<<endl;
                        // cout<<"Fitness New: "<<fitnessValueNew<<endl;


                        if(fitnessValueNew <= fitnessValueOld){
                            //update localBest
                            for(int k=1;k<=dim;k++){
                                p[i].localBest[k] = p[i].position[k];
                                p[i].localBestV[k] = p[i].velocity[k];
                            }
                        }else {
                            // update position as previous
                            for(int k=1;k<=dim;k++){
                                p[i].position[k] = p[i].localBest[k];
                                p[i].velocity[k] = p[i].localBestV[k];
                            }
                        }

                        if(globalBestValue > fitnessValueOld){
                            //update globalBest
                            for(int k=1;k<=dim;k++){
                                globalBestSolution[k] = p[i].localBest[k];
                            }
                            globalBestValue = fitnessValueOld;
                        }
                        
                       // print updated velocity
                        // cout<<"V"<<i<<": ";
                        // for(int k=1;k<=dim;k++){
                        //     cout<<p[i].velocity[k]<<" ";
                        // }
                        
                        // print feasible solution 
                        // cout<<"S"<<i<<": ";
                        // for(int k=1;k<=dim;k++){
                        //     cout<<p[i].position[k]<<" ";
                        // }
                        // cout<<"   \n";
                        // cout<<endl;
                        // print feasible solution 
                        // cout<<"V"<<i<<": ";
                        // for(int k=1;k<=dim;k++){
                        //     cout<<p[i].velocity[k]<<" ";
                        // }
                        // cout<<endl<<endl;

                    }
                    // cout<<endl<<"--------------------------------------------------\n"<<endl;    
                }
                cout<<"Optimal Solution After "<<iter<<": ";
                for(int k=1;k<=dim;k++)cout<<globalBestSolution[k]<<" ";
                cout<<endl;
                cout<<"gBest = "<<globalBestValue<<endl;
                
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
                        if(getNeigh!=-1){
                            p[getNeigh] = 1;
                            p[i]= 1;
                        }
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
    
    int node,edge;
    cin>>node;
    cin>>edge;
    Graph G;
    while(edge--){
        int v1,v2;
        cin>>v1>>v2;
        G.addEdge(v1,v2);
    }
   
    int popSize = 1000;
    Swarm s(popSize,node,G);
    double c1=0.8, c2=0.8, w=0.5;
    s.initialise(c1,c2,w);
    s.GenerateInitialSolution();
    s.mainLoop(10000);

    return 0;
}
// g++ .\main.cpp .\Heuristic1.cpp .\Heuristic2.cpp .\Graph.cpp