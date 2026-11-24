#pragma once

#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <iomanip>



using namespace std;

class AdjacencyList {

private:
    unordered_map<int, vector<pair<int,int>>> AdjList; // We do need this list, do we need any maps for indexes?
    vector<string> PrevReSults; // So we can store results to display again in a history;


public:


    AdjacencyList();
    void AddData(string filename); // So we can input all of our data, or are we going to hardcode it?
    void InsertEdge(string from, string to, int weight); // Inserts a new Edge, creates nodes to store
    void ASearch(int from, int to); // For our A* sort
    void Dsearch(int from, int to); // For our Dijkstra implementation
    void Bmark(string filename); // For testing purposes, maybe we can have it just output speed
    void DisplayHistory(); // Will just print out the last 50 requests
};


