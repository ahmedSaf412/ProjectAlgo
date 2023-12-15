#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <set>

using namespace std;

class Graph {
public:
    Graph(int vertices);
    void addEdge(int src, int dest);
    void displayGraph();
    const list<int>& getAdjList(int vertex) const; // New function to access adjList

private:
    int numVertices;
    list<int>* adjList; // Pointer to an array containing adjacency lists
};

const list<int>& Graph::getAdjList(int vertex) const {
    return adjList[vertex];
}


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
    int numAccounts = 60;

    if (outputfile.is_open()) {
        srand(static_cast<unsigned int>(time(0)));

        set<int> uniqueIDs; // To keep track of generated IDs

        for (int i = 0; i < numAccounts; ++i) {
            int ID;
            do {
                ID = 1 + rand() % numAccounts;
            } while (!uniqueIDs.insert(ID).second); // Ensure ID is unique

            outputfile << ID << endl;  // Assuming each ID should be on a new line
        }

        outputfile.close();
        cout << "Dummy IDs generated and saved to dummy_ids.txt." << endl;
    } else {
        cerr << "Unable to open file for writing." << endl;
    }
}

bool binarySearch(const vector<int>& arr, int key) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == key) {
            return true; // Key found
        } else if (arr[mid] < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return false; // Key not found
}

bool listSearch(const list<int>& lst, int key) {
    for (const auto& item : lst) {
        if (item == key) {
            return true; // Key found
        }
    }
    return false; // Key not found
}

bool binarySearch(const list<int>& lst, int key) {
    return listSearch(lst, key);
}
int main() {
    ifstream inputfile("dummy_ids.txt");
    ofstream outputfile("dummy_ids.txt");

    // Generate random dummy IDs and save them to a file
    RandomID2(outputfile);

    vector<int> userIDs;

    // Read user IDs from the file and store them in a vector
    if (inputfile.is_open()) {
        int userID;
        while (inputfile >> userID) {
            userIDs.push_back(userID);
        }
        inputfile.close();
    } else {
        cerr << "Unable to open dummy_ids.txt for reading." << endl;
        return 1;
    }

    // Sort the userIDs vector
    sort(userIDs.begin(), userIDs.end());

    Graph userGraph(61);

    srand(static_cast<unsigned int>(time(0)));

    set<int> selectedUsers; // To keep track of selected user IDs

    // Assuming you want to add some edges to represent "follow" relationships
 for (int j = 0; j < 1000; j++) {
    int randomUser = userIDs[0 + rand() % (userIDs.size())];
    int randomUser2 = userIDs[0 + rand() % (userIDs.size())];

    // Check if the edge already exists in the graph
// Check if the edge already exists in the graph
if (randomUser != randomUser2 &&
    !listSearch(userGraph.getAdjList(randomUser), randomUser2) &&
    !listSearch(userGraph.getAdjList(randomUser2), randomUser)) {
    userGraph.addEdge(randomUser, randomUser2);
    selectedUsers.insert(randomUser);
    selectedUsers.insert(randomUser2);
}

}


    // Display the graph
    userGraph.displayGraph();

    return 0;
}