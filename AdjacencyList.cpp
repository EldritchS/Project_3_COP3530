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


void AdjacencyList::LoadGraph(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open " << filename << std::endl;
        return;
    }
    std::string line;
    bool first = true;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] != 'a') continue;   // skip comments
        std::stringstream ss(line);
        char type;
        long long from, to;
        double weight;
        ss >> type >> from >> to >> weight;
        InsertEdge(from, to, weight);

        // A* needs its guess to never be bigger than the real distance.
        // The file's lengths aren't exactly meters, so we find the smallest
        // (road length / straight-line meters) ratio and multiply the guess by it.
        double meters = Haversine(from, to);
        if (meters > 0 && (first || weight / meters < scale)) {
            scale = weight / meters;
            first = false;
        }
    }
}

void AdjacencyList::DisplayHistory() {
    if (history.empty()) {
        std::cout << "No history yet." << std::endl;
        return;
    }
    for (int i = 0; i < (int)history.size(); i++)
        std::cout << i + 1 << ". " << history[i] << std::endl;
}
