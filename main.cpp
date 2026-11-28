#include <iostream>
#include <string>
#include <sstream>
#include "AdjacencyList.h"


using namespace std;
void Intro(AdjacencyList& list){ 
cout << "Loading Map Data" << endl << endl;
Parse(list);

cout << "________________________________________" << endl;
cout << "Welcome to our distance calculator." << endl;
cout << "Not a GPS, but good to plan around." << endl;
cout << "Made to compare A* and Dijkstra's algs." << endl;
cout << "________________________________________" << endl << endl;

}

void Parse(AdjacencyList& list){
while (list.VertexCount() == 0) {
        std::string coFile, grFile;
        std::cout << "Enter the path to the coordinates file (.co): ";
        std::getline(std::cin, coFile);
        std::cout << "Enter the path to the roads file (.gr): ";
        std::getline(std::cin, grFile);

        list.LoadCoordinates(CleanPath(coFile));
        list.LoadGraph(CleanPath(grFile));

        if (list.VertexCount() == 0)
            std::cout << "Nothing was loaded, please try again." << std::endl << std::endl;
    }
    std::cout << "Loaded " << list.VertexCount() << " vertices and " << list.EdgeCount() << " edges." << std::endl;
}

std::string CleanPath(std::string path) {
    while (!path.empty() && (path.front() == '"' || path.front() == ' ')) path.erase(0, 1);
    while (!path.empty() && (path.back() == '"' || path.back() == ' ' || path.back() == '\r')) path.pop_back();
    return path;
}

int main() {
    AdjacencyList list;
    Intro(list);

    long long from = -1;
    long long to = -1;

    while (true) {
        std::cout << std::endl;
        std::cout << "1. Enter start vertex" << std::endl;
        std::cout << "2. Enter end vertex" << std::endl;
        std::cout << "3. Run algorithms and compare" << std::endl;
        std::cout << "4. Print history" << std::endl;
        std::cout << "5. Run benchmark" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cout << "Enter an input: ";

        std::string input;
        std::cin >> input;

        if (input == "1" || input == "2") {
            long long id;
            std::cout << "Vertex ID (1 to " << list.VertexCount() << "): ";
            std::cin >> id;
            if (!list.HasVertex(id)) {
                std::cout << "That vertex doesn't exist." << std::endl;
                std::cin.clear();
                std::cin.ignore(1000, '\n');
            }
            else if (input == "1") from = id;
            else to = id;
        }
        else if (input == "3") {
            if (from == -1 || to == -1) std::cout << "Enter a start and end vertex first." << std::endl;
            else list.Compare(from, to);
        }
        else if (input == "4") {
            list.DisplayHistory();
        }
        else if (input == "5") {
            int n;
            std::cout << "How many random queries? ";
            std::cin >> n;
            if (n > 0) list.Bmark("results.csv", n);
        }
        else if (input == "6") {
            break;
        }
        else {
            std::cout << "Invalid input, please try again." << std::endl;
        }
    }
    return 0;
}
