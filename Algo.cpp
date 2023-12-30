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

    const unordered_map<int, vector<int>>& getAdjacencyList() const {
        return adjacencyList;
    }

private:
    unordered_map<int, vector<int>> adjacencyList;
};



// Utility function to get the maximum value in a pair of integers
int getMaxValue(const vector<pair<int, int>>& arr) {
    int maxVal = INT_MIN;
    for (const auto& pair : arr) {
        maxVal = max(maxVal, pair.first);
    }
    /*for (int i{0}; i <= arr.size(); i++) {
        maxVal=max(arr[i].first,arr[i+1].first);
    }
    */
    return maxVal;
}


// Radix sort implementation for pairs of integers
void radix_sort(vector<pair<int, int>>& arr) {
    const int RADIX = 10;
    int exp = 1;
    int n = arr.size();
    vector<pair<int, int>> output(n);
    int maxVal = getMaxValue(arr);

    while (maxVal / exp > 0) {
        int count[RADIX] = { 0 };

        // Count the occurrences of each digit at the current exp
        for (int i = 0; i < n; i++)
            count[(arr[i].second / exp) % RADIX]++;

        // Accumulate the count array
        for (int i = 1; i < RADIX; i++)
            count[i] += count[i - 1];

        // Build the output array in reverse order to get descending order
        for (int i = 0; i < n; i++) {
            output[count[(arr[i].second / exp) % RADIX] - 1] = arr[i];
            count[(arr[i].second / exp) % RADIX]--;
        }

        // Copy the output array to arr so that arr contains sorted numbers according to the current digit
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
    }

    file.close();
}

vector<int> recommendFollowers(const unordered_map<int, vector<int>>& adjacencyList, int userId) {
    unordered_set<int> followersSet;

    // Get followers of the user
    auto it = adjacencyList.find(userId);
    if (it != adjacencyList.end()) {
        followersSet.insert(it->second.begin(), it->second.end());
    }

    // Recommend followers based on common followers
    unordered_map<int, int> commonFollowersCount;
    for (const auto& follower : followersSet) {
        // Use 'at()' instead of '[]' to access the inner vector
        for (const auto& recommendedUser : adjacencyList.at(follower)) {
            // Exclude users already followed
            if (followersSet.find(recommendedUser) == followersSet.end()) {
                commonFollowersCount[recommendedUser]++;
            }
        }
    }

    // Convert the unordered_map to a vector of pairs
    vector<pair<int, int>> recommendedUsers(commonFollowersCount.begin(), commonFollowersCount.end());

    // Sort recommended users by the number of common followers using radix sort
    radix_sort(recommendedUsers);

    // Extract recommended user IDs
    vector<int> recommendedUserIds;
    for (const auto& entry : recommendedUsers) {
        recommendedUserIds.push_back(entry.first);
    }

    return recommendedUserIds;
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

    vector<int> recommendedUsers = recommendFollowers(graph.getAdjacencyList(), userId);

    cout << "Recommended users to follow:" << endl;
    for (size_t i = 0; i < min(recommendedUsers.size(), static_cast<size_t>(10)); ++i) {
        cout << "ID: " << recommendedUsers[i] << endl;
    }

    return 0;
}
