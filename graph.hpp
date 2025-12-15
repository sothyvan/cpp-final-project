#ifndef graph_hpp
#define graph_hpp

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <limits>
#include <iomanip>

using namespace std;

// ==================== GRAPH DATA STRUCTURE ====================

class Graph {
private:
    struct Edge {
        int destination;
        double weight;  // distance in km or time in minutes
        
        Edge(int dest, double w) : destination(dest), weight(w) {}
    };
    
    // Adjacency list representation
    vector<vector<Edge>> adjacencyList;
    
    // Map to store node names (delivery locations)
    vector<string> locationNames;
    
    // Map location names to indices
    unordered_map<string, int> locationToIndex;
    
public:
    // Constructor
    Graph() {}
    
    // Add a location to the graph
    void addLocation(const string& name) {
        if (locationToIndex.find(name) == locationToIndex.end()) {
            locationToIndex[name] = adjacencyList.size();
            locationNames.push_back(name);
            adjacencyList.push_back(vector<Edge>());
        }
    }
    
    // Add a route between two locations
    void addRoute(const string& from, const string& to, double distance) {
        int fromIndex = getLocationIndex(from);
        int toIndex = getLocationIndex(to);
        
        // Add edges in both directions (undirected graph)
        adjacencyList[fromIndex].push_back(Edge(toIndex, distance));
        adjacencyList[toIndex].push_back(Edge(fromIndex, distance));
    }
    
    // Get index of a location
    int getLocationIndex(const string& name) {
        if (locationToIndex.find(name) == locationToIndex.end()) {
            addLocation(name);
        }
        return locationToIndex[name];
    }
    
    // Get location name from index
    string getLocationName(int index) {
        return locationNames[index];
    }
    
    // Get number of locations
    int getNumLocations() {
        return locationNames.size();
    }
    
    // Display the graph structure
    void displayGraph() {
        cout << "\n========== DELIVERY NETWORK GRAPH ==========\n";
        for (int i = 0; i < adjacencyList.size(); i++) {
            cout << locationNames[i] << " -> ";
            for (const Edge& edge : adjacencyList[i]) {
                cout << locationNames[edge.destination] << "(" << edge.weight << " km) ";
            }
            cout << endl;
        }
    }
    
    // ==================== DIJKSTRA'S ALGORITHM ====================
    
    vector<int> findShortestPath(const string& start, const string& end) {
        int startIndex = getLocationIndex(start);
        int endIndex = getLocationIndex(end);
        
        int n = adjacencyList.size();
        vector<double> distances(n, numeric_limits<double>::max());
        vector<int> previous(n, -1);
        vector<bool> visited(n, false);
        
        // Priority queue for Dijkstra's algorithm
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        
        distances[startIndex] = 0;
        pq.push({0, startIndex});
        
        while (!pq.empty()) {
            int current = pq.top().second;
            pq.pop();
            
            if (visited[current]) continue;
            visited[current] = true;
            
            if (current == endIndex) break;
            
            for (const Edge& edge : adjacencyList[current]) {
                int neighbor = edge.destination;
                double newDist = distances[current] + edge.weight;
                
                if (newDist < distances[neighbor]) {
                    distances[neighbor] = newDist;
                    previous[neighbor] = current;
                    pq.push({newDist, neighbor});
                }
            }
        }
        
        // Reconstruct the path
        vector<int> path;
        if (distances[endIndex] == numeric_limits<double>::max()) {
            return path;  // Empty path (no route found)
        }
        
        for (int at = endIndex; at != -1; at = previous[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
        
        return path;
    }
    
    // Get total distance of a path
    double getPathDistance(const vector<int>& path) {
        if (path.size() < 2) return 0;
        
        double totalDistance = 0;
        for (size_t i = 0; i < path.size() - 1; i++) {
            int from = path[i];
            int to = path[i+1];
            
            // Find the edge weight between these nodes
            for (const Edge& edge : adjacencyList[from]) {
                if (edge.destination == to) {
                    totalDistance += edge.weight;
                    break;
                }
            }
        }
        return totalDistance;
    }
    
    // ==================== DFS FOR ALTERNATIVE ROUTES ====================

    void findAllRoutes(const string& start, const string& end, const vector<int>& shortestPath, int maxRoutes = 5) {
        int startIndex = getLocationIndex(start);
        int endIndex = getLocationIndex(end);
        
        vector<vector<int>> allPaths;
        vector<int> currentPath;
        unordered_set<int> visited;
        
        // Find ALL possible paths (or a reasonable number)
        // Increase limit to ensure we find enough alternatives
        int pathLimit = 20;  // Find up to 20 paths total
        dfsFindAllPaths(startIndex, endIndex, visited, currentPath, allPaths, pathLimit);
        
        // Filter out the shortest path and create pairs of (path, distance)
        vector<pair<vector<int>, double>> alternativePathsWithDistance;
        for (const auto& path : allPaths) {
            if (path != shortestPath) {
                double distance = getPathDistance(path);
                alternativePathsWithDistance.push_back({path, distance});
            }
        }
        
        // Sort all alternative paths by distance (shortest to longest)
        sort(alternativePathsWithDistance.begin(), alternativePathsWithDistance.end(),
            [](const pair<vector<int>, double>& a, const pair<vector<int>, double>& b) {
                return a.second < b.second;
            });
        
        cout << "\n========== ALTERNATIVE ROUTES (SHORTEST TO LONGEST) ==========\n";
        
        if (alternativePathsWithDistance.empty()) {
            cout << "No alternative routes found.\n";
        } else {
            // Take the top maxRoutes alternatives (shortest ones)
            int displayCount = min((int)alternativePathsWithDistance.size(), maxRoutes);
            
            for (int i = 0; i < displayCount; i++) {
                cout << "Alternative Route " << (i+1) << ": ";
                displayPath(alternativePathsWithDistance[i].first);
                cout << "Total distance: " << fixed << setprecision(1) << alternativePathsWithDistance[i].second << " km\n\n";
            }
        }
    }

private:
    void dfsFindAllPaths(int current, int end, unordered_set<int>& visited, 
                        vector<int>& currentPath, vector<vector<int>>& allPaths, int maxPaths) {
        // To avoid infinite loops in cyclic graphs, limit path length
        if (currentPath.size() > 10) {  // Don't explore paths longer than 10 nodes
            return;
        }
        
        visited.insert(current);
        currentPath.push_back(current);
        
        if (current == end) {
            allPaths.push_back(currentPath);
        } else {
            for (const Edge& edge : adjacencyList[current]) {
                if (visited.find(edge.destination) == visited.end() && allPaths.size() < maxPaths) {
                    dfsFindAllPaths(edge.destination, end, visited, currentPath, allPaths, maxPaths);
                }
            }
        }
        
        currentPath.pop_back();
        visited.erase(current);
    }
    
    void displayPath(const vector<int>& path) {
        for (size_t i = 0; i < path.size(); i++) {
            cout << locationNames[path[i]];
            if (i != path.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    }
};

#endif