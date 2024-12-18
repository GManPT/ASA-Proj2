// // Created by Leonardo Neves (110597) and Vicente Duarte (109939)

#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <queue>

using namespace std;

int n, m, l; // Number of stations, connections and lines

// create a vector of vectors for a graph
vector<set<int>> GRAPH;

// create another graph for input information
vector<unordered_set<int>> INPUTGRAPH;

// create a vector and an integer variable to keep track of empty stations
int emptyStations;
vector<int> workingStations;
vector<int> stationsPerLine;

// result of 1st bfs 
int farthestNeighbor;

/// Read input values from stdin
/// @return none
int readInput() {

    // Fast I/O (Leaks memory)
    ios::sync_with_stdio(0);
    cin.tie(0);

    // Read n (stations), m (connections) and l (lines) values
    cin >> n >> m >> l;

    // Resize global variables
    GRAPH.resize(l, set<int>());
    INPUTGRAPH.resize(n, unordered_set<int>());

    emptyStations = n;
    int input = 1;
    workingStations.resize(n);
    stationsPerLine.resize(l);

    // Read input values
    for (int i = 0; i < m; i++) {
        int s1, s2, cl;
        cin >> s1 >> s2 >> cl;

        // add s1 and s2 to workingStationsVector
        if (workingStations[s1 - 1] == 0) {
            workingStations[s1 - 1] = 1;
            emptyStations--;
        }
        if (workingStations[s2 - 1] == 0) {
            workingStations[s2 - 1] = 1;
            emptyStations--;
        }
        
        auto res1 = INPUTGRAPH[s1 - 1].insert(cl);
        if (res1.second) { // in case the insertion was successful
            if (++stationsPerLine[cl - 1] == n) input = 0;
            for (int x : INPUTGRAPH[s1 - 1]) { // for each element of the set
                if (x == cl) continue;
                GRAPH[x - 1].insert(cl);
                GRAPH[cl - 1].insert(x);
            }
        }
        
        auto res2 = INPUTGRAPH[s2 - 1].insert(cl);
        if (res2.second) {
            if (++stationsPerLine[cl - 1] == n) input = 0;
            for (int x : INPUTGRAPH[s2 - 1]) {
                if (x == cl) continue;
                GRAPH[x - 1].insert(cl);
                GRAPH[cl - 1].insert(x);
            }
        }
    }

    if (input == 0) {
        // there is a line that runs through every station
        return 0;
    }

    if (emptyStations > 0) {
        // there are stations with no lines
        return -1;
    } else {
        // the input read was successful
        return input;
    }
}


/// Breadth-first search algorithm
/// @param start the starting node
/// @return the maximum distance from the starting node
int bfs(int start) {
    int totalNodes = l - 1;
    vector<bool> visited(l, false);
    vector<int> distance(l, 0);
    queue<int> q;

    visited[start - 1] = true;
    q.push(start);

    int maxDistance = 0;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbor : GRAPH[node - 1]) {
            if (!visited[neighbor - 1]) {
                visited[neighbor - 1] = true;
                distance[neighbor - 1] = distance[node - 1] + 1;
                maxDistance = max(maxDistance, distance[neighbor - 1]);
                q.push(neighbor);

                // update farthestNeighbor
                farthestNeighbor = neighbor;
                totalNodes--;
            }
        }
    }
    
    if (totalNodes > 0) {
        return -1;
    }
    else {
        return maxDistance;
    }
}

/// Calculate the distance between the stations
/// @param input the input value
/// @return none
void calculateDistance(int input) {
    // if the input is invalid or the bfs returns -1, print -1
    if (input == -1 || input == 0) {
        cout << input << endl;
        return;
    }

    if (bfs(1) == -1) {
        cout << -1 << endl;
        return;
    }

    // Get the farthest neighbor from the first bfs and run the bfs again
    cout << bfs(farthestNeighbor) << endl;
}

int main () {
    int input = readInput();
    calculateDistance(input);
    return 0;
}