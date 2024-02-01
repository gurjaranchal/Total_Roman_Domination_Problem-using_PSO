// Heuristic1.h
#ifndef HEURISTIC1_H
#define HEURISTIC1_H

#include "Graph.h"

class Heuristic1 {
private:
    Graph graph;
    double* labelledSet;
    int n;

public:
    Heuristic1(const Graph& g, int size);
    std::vector<int> findMaxDegreeNode();
    void labelNeighbours(int node);
    bool FindLabelNegative(double* l);
    void findLabelledSet();
    double* getLabelledSet();
};

#endif // HEURISTIC1_H
