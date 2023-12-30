#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;

class Graph {
public:
    void FollowReg(int follower, int followee) {
        adjacencyList[followee].push_back(follower);
    }
    void Followings(int follower, int followee) {
        adjacencyList2[follower].push_back(followee);
    }

    const unordered_map<int, vector<int>>& getAdjacencyList() const {
        return adjacencyList;
    }

private:
    unordered_map<int, vector<int>> adjacencyList;
    unordered_map<int, vector<int>> adjacencyList2;
};

int getMaxValue(const vector<pair<int, int>>& arr) {
    int maxVal = INT_MIN;
    for (const auto& pair : arr) {
        maxVal = max(maxVal, pair.first);
    }
    return maxVal;
}

void radix_sort(vector<pair<int, int>>& arr) {
    const int RADIX = 10;
    int exp = 1;
    int n = arr.size();
    vector<pair<int, int>> output(n);
    int maxVal = getMaxValue(arr);

    while (maxVal / exp > 0) {
        int count[RADIX] = { 0 };

        for (int i = 0; i < n; i++)
            count[(arr[i].second / exp) % RADIX]++;

        for (int i = 1; i < RADIX; i++)
            count[i] += count[i - 1];

        for (int i = 0; i < n; i++) {
            output[count[(arr[i].second / exp) % RADIX] - 1] = arr[i];
            count[(arr[i].second / exp) % RADIX]--;
        }

        for (int i = 0; i < n; i++)
            arr[i] = output[i];

        exp *= RADIX;
    }
}

void readCSV(const string& filename, Graph& graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening the file.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string followerStr;
        string followeeStr;

        getline(ss, followeeStr, ',');
        int from = stoi(followeeStr);

        getline(ss, followerStr, ',');
        int to = stoi(followerStr);

        graph.FollowReg(from, to);
        graph.Followings(to, from);
    }

    file.close();
}

vector<pair<int, int>> recommendFollowee(int userID, const unordered_map<int, vector<int>>& adjacencyList) {
    vector<pair<int, int>> frequencyOfUser;

    // Check if the userID exists in the adjacencyList
    auto userIt = adjacencyList.find(userID);
    if (userIt == adjacencyList.end()) {
        cout << "User ID: " << userID << " not found in the graph." << endl;
        return frequencyOfUser;  // Return an empty vector if user not found
    }

    // Count the frequency of each followee of the given user
    unordered_map<int, int> followeeCount;
    for (int followee : userIt->second) {
        auto followeeIt = adjacencyList.find(followee);
        if (followeeIt != adjacencyList.end()) {
            for (int user : followeeIt->second) {
                // Check if user is not the given user and not already followed
                if (user != userID && find(userIt->second.begin(), userIt->second.end(), user) == userIt->second.end()) {
                    followeeCount[user]++;
                }
            }
        }
    }

    // Convert the followeeCount map to a vector of pairs
    for (const auto& entry : followeeCount) {
        frequencyOfUser.emplace_back(entry.first, entry.second);
    }

    // Sort the vector by the count of followees using radix sort
    radix_sort(frequencyOfUser);

    return frequencyOfUser;
}


vector<pair<int, int>> getTopFollowers(const unordered_map<int, vector<int>>& adjacencyList) {
    vector<pair<int, int>> followersCount;

    for (const auto& entry : adjacencyList) {
        followersCount.emplace_back(entry.first, entry.second.size());
    }

    // Sort followers count by the number of followers using radix sort
    radix_sort(followersCount);

    return followersCount;
}

int main() {
    Graph graph;
    readCSV("twitter.csv", graph);

    cout << "Graph created successfully." << endl;

    // Get user input for the number of top users to be shown
    int topUsersCount;
    cout << "Enter the number of top users to be shown: ";
    cin >> topUsersCount;

    const auto topFollowers = getTopFollowers(graph.getAdjacencyList());

    cout << "Top people with most followers:" << endl;
    for (size_t i = 0; i < min(topFollowers.size(), static_cast<size_t>(topUsersCount)); ++i) {
        cout << "ID: " << topFollowers[i].first << ", Followers: " << topFollowers[i].second << endl;
    }

    // Get user input for which user to display followers count
    int userIdToCheck;
    cout << "Enter the user ID to check followers count: ";
    cin >> userIdToCheck;

    // Check if the entered user ID is valid
    auto userIt = graph.getAdjacencyList().find(userIdToCheck);
    if (userIt != graph.getAdjacencyList().end()) {
        // Display the number of followers for the entered user
        cout << "User ID: " << userIdToCheck << " has " << userIt->second.size() << " followers." << endl;
    }
    else {
        cout << "User ID: " << userIdToCheck << " not found in the graph." << endl;
    }

    // Get user input for which user to provide recommendations
    int userId;
    cout << "Enter the user ID for recommendations: ";
    cin >> userId;

    vector<pair<int, int>> recommendedFollowees = recommendFollowee(userId, graph.getAdjacencyList());

    cout << "Recommended followees:" << endl;
    for (size_t i = 0; i < min(recommendedFollowees.size(), static_cast<size_t>(10)); ++i) {
        cout << "ID: " << recommendedFollowees[i].first << ", Count: " << recommendedFollowees[i].second << endl;
    }

    return 0;
}
