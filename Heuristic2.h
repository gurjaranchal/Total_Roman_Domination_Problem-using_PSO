// Heuristic2.h
#ifndef HEURISTIC2_H
#define HEURISTIC2_H

#include "Graph.h"

class Heuristic2 {
private:
    Graph graph;
    double* labelledSet;
    int n;

public:
    Heuristic2(const Graph& g, int size);
    void labelNeighbours(int node);
    bool FindLabelNegative(double* l);
    int FindUnlabelledIndex(double* l,int r);
    void findLabelledSet();
    double* getLabelledSet();
    ~ Heuristic2(){
        delete[] labelledSet;

    }

};

#endif // HEURISTIC2_H
