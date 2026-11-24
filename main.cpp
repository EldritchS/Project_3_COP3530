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

}

int main() {
    AdjacencyList list;
    bool help = true;
    //If we only use the one data set, we'll likely put the parsing of it here, if we want more we should add it to our graph obj.
    int from = 1;
    int to = 0;
    int weight = 10;
    Intro(list);

    while (true) { //Because we're making a menu, I assume we need most of the code to loop
    if (help) {
        //write whatever our menu will end up looking like.
        cout << "1. Enter start Vertex" << endl;
        cout << "2. Enter End Vertex" << endl;
        cout << "3. Run Algorithms and compare" << endl;
        cout << "4. Print History" << endl;
        cout << "5. Exit" << endl;
    }

        cout << "Enter an input" << endl;
        string input;
        cin >> input;
        else if (input == "1"){

        }
        else if (input == "2"){

        }
        else if (input == "3"){

        }
        else if (input == "4"){

        }
        if (input == "5"){
            break;
        }
        else {
            cout << "Invalid input, please try again." << endl;
        }

    }
    return 0;
}
