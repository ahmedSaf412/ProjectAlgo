#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm> // Added for std::sort
#include <string>
using namespace std;

class Graph {
private:
    // No. of vertices
    int V;

    // Pointer to an array containing adjacency lists
    vector<list<int>> followings;
    vector<list<int>> followers;

public:
    // Constructor
    Graph(int V);

    // Function to add an edge to the graph
    void addEdge(int src, int dist);

    // Function to sort rich people based on followers count
    void sortRichPeople();
    void mergeSort(vector<int>& followers, int l, int r);
    void merge(vector<int>& followers, int l, int m, int r);

    void TopInfluencers(int TopSelected);
};

Graph::Graph(int V) {
    this->V = V;
    followings.resize(V);
    followers.resize(V);
}

void Graph::addEdge(int src, int dist) {
    // Add dist to src's list of followings
    followings[src].push_back(dist);
    // Add src to dist's list of followers
    followers[dist].push_back(src);
}

void Graph::merge(vector<int>& followers, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) {
        L[i] = followers[l + i];
    }

    for (int j = 0; j < n2; j++) {
        R[j] = followers[m + 1 + j];
    }

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] >= R[j]) {
            followers[k] = L[i];
            i++;
        }
        else {
            followers[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        followers[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        followers[k] = R[j];
        j++;
        k++;
    }
}

void Graph::mergeSort(vector<int>& followers, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(followers, l, m);
        mergeSort(followers, m + 1, r);

        merge(followers, l, m, r);
    }
}

void Graph::sortRichPeople() {
    vector<int> userFollowersCount(V);
    for (int i = 0; i < V; i++) {
        userFollowersCount[i] = followers[i].size();
    }

    // Create an index vector and sort it based on followers count using merge sort
    mergeSort(userFollowersCount, 0, V - 1);

    // Now userFollowersCount contains indices of users sorted by followers count
    // You can use this information as needed
}

void Graph::TopInfluencers(int TopSelected) {
    cout << "Top " << TopSelected << " are:\n";
    for (int i = 0; i < TopSelected; ++i) {
        cout << "User " << followers.v << " with " << followers[i].size() << " followers";
    }
}

int main() {
    ifstream inputfile("twitter.csv");

    int V;  // Assuming you need to read the number of vertices from the file
    inputfile >> V;

    Graph graph(V);

    string line;
    if (!inputfile.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    else {
        while (getline(inputfile, line)) {
            stringstream ss(line);
            string srcStr, dist;
            getline(ss, srcStr, ',');
            getline(ss, dist, ',');
            int src = stoi(srcStr);
            int dest = stoi(dist);

            graph.addEdge(src, dest);
        }

        inputfile.close();
    }

    // Example of using sortRichPeople function
    graph.sortRichPeople();

    cout << "\n\nEnter Top followers you want to see\n";
    int TopSelected;
    cin >> TopSelected; // Added this line to read the user input

    graph.TopInfluencers(TopSelected);

    return 0;
}
