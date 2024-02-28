#ifndef PARTICLE_H
#define PARTICLE_H
using namespace std;
class Particle{
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
        // initial velocity
        for(int i=0;i<d;i++){
            velocity[i] = 0.1; 
            localBestV[i] = 0.1;
        }
    }
};


#endif