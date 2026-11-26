#include "AdjacencyList.h"
#include <algorithm>
#include <chrono>
#include <queue>
#include <unordered_set>

// Dijkstra: always look at the closest unvisited vertex next.
Result AdjacencyList::Dsearch(long long from, long long to) {
    auto start = std::chrono::steady_clock::now();
    Result result;

    std::unordered_map<long long, double> dist; // shortest distance found so far
    std::unordered_map<long long, long long> prev; // the vertex we came from
    std::unordered_set<long long> visited;

    // std::min-heap of (distance, vertex)
    std::priority_queue<std::pair<double, long long>, std::vector<std::pair<double, long long>>, std::greater<std::pair<double, long long>>> pq;
    dist[from] = 0;
    pq.push({0, from});