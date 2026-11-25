#include "AdjacencyList.h"
#include <fstream>
#include <iostream>
#include <sstream>

void AdjacencyList::InsertVertex(long long id, double lat, double lon) {
    Coords[id] = {lat, lon};
    AdjList[id]; // make an empty list of roads for this vertex
    vertices.push_back(id);
}

void AdjacencyList::InsertEdge(long long from, long long to, double weight) {
    AdjList[from].push_back({to, weight});// one-way: from -> to
    edgeCount++;
}
