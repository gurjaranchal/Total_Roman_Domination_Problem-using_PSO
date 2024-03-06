#include <bits/stdc++.h>
#include "Heuristic.h"
#include "Heuristic2.h"
#include "Heuristic1.h"
#include "Particle.h"
using namespace std;
class Swarm{
        Graph graph;
        double* globalBestSolution;
        int globalBestValue;
        Particle* p;
        int t,minIndex,popsize, dim;
    public:
    Swarm(int n,int d,const Graph& g) : graph(g){
        popsize = n;
        dim =d;
        globalBestSolution = new double[d+1];
        minIndex = -1;
        p = new Particle[n];
        t=0;
        for(int i=0;i<n;i++){
            p[i].init(d+1);
        }
      }
      void GenerateInitialSolution(){
            int minimumObjective = INT_MAX;
            // apply Heuristic1 to generate Solutions
            for(int itr = 0;itr < popsize; itr++){
                Heuristic H1(graph,dim);
                H1.findLabelledSet();
                double* l = H1.getLabelledSet();
                copy(l, l + dim + 1, p[itr].position);
                int obj = ObjectiveValue(p[itr].position);
                if(minimumObjective > obj){
                    minimumObjective = obj;
                    minIndex = itr;
                }
                p[itr].localBest = p[itr].position;
            //     cout<<"S:"<<itr<<"  ";
            //    H1.printSolution(p[itr].position);
            }
            
            // apply Heuristic2 to generate Solutions
            for(int itr = popsize;itr<popsize;itr++){
                Heuristic2 H2(graph,dim);
                H2.findLabelledSet();
                double* l = H2.getLabelledSet();
                copy(l, l + dim + 1, p[itr].position);
                int obj = ObjectiveValue(p[itr].position);
                if(minimumObjective > obj){
                    minimumObjective = obj;
                    minIndex = itr;
                }
                p[itr].localBest = p[itr].position;
                // cout<<"S"<<itr<<":  ";
                // H2.printSolution(p[itr].position);
            }
            globalBestValue = minimumObjective;
            copy(p[minIndex].position, p[minIndex].position + dim + 1, globalBestSolution);
            cout<<"After Initial Solution Gbest Solution is: ";
            for(int i=1;i<=dim;i++)cout<<globalBestSolution[i]<<" "; 
            cout<<"GBest: "<<globalBestValue<<endl<<"--------------------------------------------------\n"<<endl;                     
        }
        int ObjectiveValue(double* p){
            int sum=0;
            for(int i=1;i<=dim;i++){
                sum+= static_cast<int>(p[i]);
            }
            return sum;
        }
        int fun(double v){
            double sig = 1.0/(1.0 +exp(-v));
            double r1 = ((double)rand() / (RAND_MAX));
            double r=0.5;
            if(r1>r && r<sig){
                return 0;
            }else if(r1<r && r1<sig){
                return 1;
            }else
            return 2;
        }

        void mainLoop(int iter){
             random_device rd;
             mt19937 generator(rd());
            int gind = minIndex;
            double c1=0.8, c2=0.8, w=0.4,r1=0.2,r2=0.5;
            for(int it=1;it<iter;it++){
                // cout<<"Solution After "<<it<<" iteration\n"<<endl;
                for(int i=0;i<popsize;i++){
                    // double r1 = ((double)rand() / (RAND_MAX));
                    // double r2 = ((double)rand() / (RAND_MAX));
                    for(int j=1;j<=dim;j++){
                            double a=(w * p[i].velocity[j]) + (c1 * r1 * (p[i].localBest[j] - p[i].position[j])) +
                                (c2 * r2 * (globalBestSolution[j] - p[i].position[j]));
                            p[i].velocity[j] = a;
                            p[i].position[j] = p[i].position[j] + (a);
                            p[i].position[j] = SetThreshold(p[i].position[j]);
                        }
                        // for(int k=1;k<=dim;k++)cout<<p[i].position[k]<<" ";cout<<"--- ";
                        if(!CheckFeasible(p[i].position))
                           MakeFeasible(p[i].position);

                        // ReduceLabels(p[i].position);
                        // for(int k=1;k<=dim;k++)cout<<p[i].position[k]<<" ";cout<<endl;
                        int fitnessValueOld = ObjectiveValue(p[i].localBest),fitnessValueNew = ObjectiveValue(p[i].position);
                        // cout<<"   \n";
                        // cout<<"f: "<<fitnessValueNew<<" ";
                        if(fitnessValueNew < fitnessValueOld){
                            //update localBest
                            for(int k=1;k<=dim;k++){
                                p[i].localBest[k] = p[i].position[k];
                                p[i].localBestV[k] = p[i].velocity[k];
                            }
                        }else{
                            //if solution is not better again update the position as previous
                            for(int k=1;k<=dim;k++){
                                p[i].position[k] = p[i].localBest[k];
                                 p[i].velocity[k] =p[i].localBestV[k];
                            }
                        }
                        // cout<<"V"<<i<<": ";
                        // for(int k=1;k<=dim;k++)cout<<p[i].velocity[k]<<" ";
                        if(globalBestValue > fitnessValueNew){
                            cout<<"Previous gBest = "<<globalBestValue<<",  ";
                            cout<<"New gBest = "<<fitnessValueNew<<endl;
                            gind = i;
                            // for(int k=1;k<=dim;k++)cout<<p[i].position[k]<<" ";
                            // cout<<endl;
                            for(int k=1;k<=dim;k++) globalBestSolution[k] = p[i].position[k];
                            globalBestValue = fitnessValueNew;
                        }
                    }
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

        bool HasNeighbourwithLabel1(int l,double* p){
            vector<int> getNeigh = graph.neighbors(l);
            for(auto it:getNeigh){
                if(p[it]==1 || p[it]==2)return true;
            }
            return false;
        }
      
            
        void ReduceLabels(double* l){
            int c=0;
             for(int i=1;i<=dim;i++){
               if(l[i]==2){
                // c++;
                vector<int> neigh = graph.neighbors(i);
                for(auto it:neigh){
                    if(l[it]==2){
                        c++;
                        l[it]=0;
                        if(!CheckFeasible(l)){
                            l[it]==2;
                        }
                    
                    }

                }
                //has no neighbour zero
                // if(HasNeighbourZero(l[i],l)==-1){
                //    l[i] = 1;
                // }
               }
            }
        }

        bool HasNeighbourwithLabel2(int l,double* p){
            vector<int> getNeigh = graph.neighbors(l);
            for(auto it:getNeigh){
                if(p[it]==2)
                return true;
            }
            return false;
        }
        int HasNeighbourZero(int l,double* p){
            vector<int> getNeigh = graph.neighbors(l);
            for(auto it:getNeigh){
                if(p[it]==0 || p[it]==1)
                return it;
            }
            return -1;
        }
        bool CheckFeasible(double* p){
            for(int i=1;i<=dim;i++){
                bool neigh1 = HasNeighbourwithLabel1(i,p);
                bool neigh2 = HasNeighbourwithLabel2(i,p);
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
                bool neigh1 = HasNeighbourwithLabel1(i,p);
                bool neigh2 = HasNeighbourwithLabel2(i,p);
                if(p[i]==2 or p[i]==1){
                    if(!neigh1 && !neigh2){
                        int getNeigh = HasNeighbourZero(i,p);
                        if(getNeigh!=-1){
                            p[getNeigh] = 1;
                        }
                    }
                }else
                {
                    if(neigh2)continue;
                    if(!neigh1 && !neigh2){
                        int hasNeigh = HasNeighbourZero(i,p);
                        if(hasNeigh!=-1) p[hasNeigh] =1;
                    }
                    p[i]=1;
                }
            }      
        }
};

int main() {
    ifstream inputFile("./test.txt");
    if(!inputFile.is_open()){
        cerr << "Error opening the file!" << std::endl;
        return 1; // Return an error code
    }
    int value,t=0,node,edge;
     while (inputFile >> value) {
        t++;
        if(t==1)node = value;
        else if(t==2){
            edge = value; break;
        }
    }
    Graph G;
    while(inputFile >> value){
        int v1 = value;
        inputFile >> value;
        int v2 = value;
        if(v1!=v2)
        G.addEdge(v1,v2);
    }
    inputFile.close();
    // int deltaMax = G.findMaxDegree(),deltaMin = G.findMinDegree();
    // double lowerBound =((double)2*node/(double)(1+deltaMax));//upperBound = (double)node/2.0; //for domination number
    // double upperBound =((double)2*node*((double)(2+(double)(log(1+deltaMin)/2))/1+deltaMin)) - 1;
    // cout<<"Lower Bound: "<<lowerBound<<","<<"Upper Bound: "<<upperBound<<endl;
    Swarm s(500,node,G);
    s.GenerateInitialSolution();
    s.mainLoop(500);
    return 0;
}

// 2γ(G)≤γtR(G)≤3γ(G).

// 