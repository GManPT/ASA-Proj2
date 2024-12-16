// // Created by Leonardo Neves (110597) and Vicente Duarte (109939)

#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

int n, m, l; // Number of stations, connections and lines

// create a vector of vectors for a graph
vector<vector<int>> graph;
vector<vector<int>> mainGraph;

// create another graph for input information
vector<vector<int>> inputGraph;
vector<vector<int>> auxiliarInputGraph;

// create a vector and an integer variable to keep track of empty stations
int emptyStations;
vector<int> workingStations;

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
    graph.resize(l, vector<int>(l));
    mainGraph.resize(l);
    inputGraph.resize(n + 1, vector<int>(l));
    auxiliarInputGraph.resize(n);

    emptyStations = n;
    int input = 1;
    workingStations.resize(n);
    

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
        

        if (inputGraph[s1 - 1][cl - 1] == 0) { // in case this station is not connected to this line yet
            inputGraph[s1 - 1][cl - 1] = 1; // mark this station as connected to this line
            if (++inputGraph[n][cl - 1] == n) input = 0;
            for (int j : auxiliarInputGraph[s1 - 1]) { // for each line that this station is connected to
                if (graph[cl - 1][j - 1] == 0) {  // if the line is not connected with new line yet
                    graph[cl - 1][j - 1] = 1; // connect the lines
                    graph[j - 1][cl - 1] = 1;
                    mainGraph[cl - 1].push_back(j); // add the line to the main graph
                    mainGraph[j - 1].push_back(cl);
                }
                
            }
            auxiliarInputGraph[s1 - 1].push_back(cl);
        }
        
        if (inputGraph[s2 - 1][cl - 1] == 0) { // same process as above (for the 2nd station of the input line)
            inputGraph[s2 - 1][cl - 1] = 1;
            if (++inputGraph[n][cl - 1] == n) input = 0;
            for (int j : auxiliarInputGraph[s2 - 1]) {
                if (graph[cl - 1][j - 1] == 0) {
                    graph[cl - 1][j - 1] = 1;
                    graph[j - 1][cl - 1] = 1;
                    mainGraph[cl - 1].push_back(j);
                    mainGraph[j - 1].push_back(cl);
                }
            }
            auxiliarInputGraph[s2 - 1].push_back(cl);
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
    vector<int> distance(l, -1);
    queue<int> q;

    visited[start] = true;
    distance[start] = 0;
    q.push(start);

    int maxDistance = 0;

    while (!q.empty()) {
        int node = q.front();
        q.pop();


        for (int neighbor : mainGraph[node - 1]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distance[neighbor] = distance[node] + 1;
                maxDistance = max(maxDistance, distance[neighbor]);
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