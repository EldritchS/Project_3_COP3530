#pragma once

#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <iomanip>


// One road going out of a verte
struct Edge {
    long long to;
    double weight;
};

// What a search returns
struct Result {
    bool found = false;
    double distance = -1;
    int expanded; // how many vertices the algorithm looked at
    double ms = 0; // how long it took
    std::vector<long long> path;
};

Class AdjacencyList {
private:
    std::unordered_map<long long, std::vector<Edge>> Adjlist; //vertex -> roads out of it
    std::unordered_map<long long, std::pair<double, double>> Coords;   // vertex -> (latitude, longitude)
    std::vector<long long> vertices;                              // list of all vertex IDs
    std::vector<std::string> history;                                  // past comparisons
    int edgeCount = 0;
    double scale = 1;   // converts meters into the graph's distance units (for A*)

    public:
    void LoadCoordinates(std::string filename); // .co file
    void LoadGraph(std::string filename);  // .gr file
    void InsertVertex(long long id, double lat, double lon);
    void InsertEdge(long long from, long long to, double weight);
    bool HasVertex(long long id);
    int VertexCount();
    int EdgeCount();

    double Haversine(long long a, long long b); // straight-line distance in meters
    Result Dsearch(long long from, long long to); // Dijkstra
    Result ASearch(long long from, long long to); // A*

    void Compare(long long from, long long to);
    void Bmark(std::string filename, int queries);
    void DisplayHistory();
};

}
