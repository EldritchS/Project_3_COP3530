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

bool AdjacencyList::HasVertex(long long id) { return Coords.count(id) > 0; }
int AdjacencyList::VertexCount() { return vertices.size(); }
int AdjacencyList::EdgeCount() { return edgeCount; }


void AdjacencyList::LoadCoordinates(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] != 'v') continue; // skip comments
        std::stringstream ss(line);
        char type;
        long long id, x, y;
        ss >> type >> id >> x >> y;
        InsertVertex(id, y / 1000000.0, x / 1000000.0);
    }
}