#include <iostream>
#include <fstream>
#include <list>
#include <cstdlib>
#include <ctime>

using namespace std;

class Graph {
public:
    Graph(int vertices);
    void addEdge(int src, int dest);
    void displayGraph();

private:
    int numVertices;
    list<int>* adjList; // Pointer to an array containing adjacency lists
};

Graph::Graph(int vertices) {
    numVertices = vertices;
    adjList = new list<int>[vertices];
}

void Graph::addEdge(int src, int dest) {
    // Check if source and destination vertices are within the valid range
    if (src >= 0 && src < numVertices && dest >= 0 && dest < numVertices) {
        // Add an edge from source to destination
        adjList[src].push_back(dest);
    } else {
        cerr << "Invalid vertices: (" << src << ", " << dest << ")" << endl;
    }
}

void Graph::displayGraph() {
    for (int i = 0; i < numVertices; ++i) {
        cout << "Vertex " << i << " neighbors:";

        for (const auto& neighbor : adjList[i]) {
            cout << " " << neighbor;
        }

        cout << endl;
    }
}



void RandomID2(ofstream& outputfile) {
    int numAccounts = 60000;

    if (outputfile.is_open()) {
        srand(static_cast<unsigned int>(time(0)));

        for (int i = 0; i < numAccounts; ++i) {
            int ID = 1 + rand() % numAccounts;
            outputfile << ID << endl;  // Assuming each ID should be on a new line
        }

        outputfile.close();
        cout << "Dummy IDs generated and saved to dummy_ids.txt." << endl;
    } else {
        cerr << "Unable to open file for writing." << endl;
    }
}



int main() {

    ifstream inputfile("dummy_ids.txt");
    ofstream outputfile("dummy_ids.txt");


    // Generate random dummy IDs and save them to a file
    RandomID2(outputfile);


    Graph userGraph(6000);


    // Assuming you want to add some edges to represent "follow" relationships

    if (inputfile.is_open()){
        srand(static_cast<unsigned int>(time(0)));
        for (int j{0};j<6000;j++) {
            
            int randomUser = rand() % 60000;
            int randomUser2 = rand() % 60000;

            userGraph.addEdge(randomUser, randomUser2); // User
        }
        inputfile.close();
    } else {
        cerr << "Unable to open dummy_ids.txt for reading." << endl;
    }

    // Display the graph
    userGraph.displayGraph();

    return 0;
}
