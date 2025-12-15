#ifndef deliveryRoute_hpp
#define deliveryRoute_hpp

#include "graph.hpp"

using namespace std;

// ==================== DELIVERY ROUTE OPTIMIZER CLASS ====================

class DeliveryRouteOptimizer {
private:
    Graph deliveryNetwork;
    
public:
    // Initialize with sample delivery network
    void initializeSampleNetwork() {
        // Add delivery locations
        vector<string> locations = {
            "Warehouse", "Downtown", "University", "Shopping Mall", 
            "Residential Area", "Industrial Park", "Airport", "Hospital"
        };
        
        for (const string& loc : locations) {
            deliveryNetwork.addLocation(loc);
        }
        
        // Add routes with distances in kilometers
        deliveryNetwork.addRoute("Warehouse", "Downtown", 5.2);
        deliveryNetwork.addRoute("Warehouse", "Industrial Park", 3.8);
        deliveryNetwork.addRoute("Downtown", "University", 2.1);
        deliveryNetwork.addRoute("Downtown", "Shopping Mall", 4.3);
        deliveryNetwork.addRoute("University", "Hospital", 3.5);
        deliveryNetwork.addRoute("Shopping Mall", "Residential Area", 2.8);
        deliveryNetwork.addRoute("Industrial Park", "Airport", 6.7);
        deliveryNetwork.addRoute("Residential Area", "Hospital", 3.2);
        deliveryNetwork.addRoute("Airport", "Hospital", 8.1);
        deliveryNetwork.addRoute("Shopping Mall", "Hospital", 4.0);
        deliveryNetwork.addRoute("University", "Residential Area", 2.9);
    }
    
    // Optimize route between two locations
    void optimizeRoute(const string& start, const string& end) {
        cout << "\n========== ROUTE OPTIMIZATION ==========\n";
        cout << "From: " << start << "  To: " << end << "\n";
        
        vector<int> shortestPath = deliveryNetwork.findShortestPath(start, end);
        
        if (shortestPath.empty()) {
            cout << "No route found between " << start << " and " << end << endl;
            return;
        }
        
        cout << "\nOPTIMAL ROUTE (Dijkstra's Algorithm):\n";
        
        for (size_t i = 0; i < shortestPath.size(); i++) {
            cout << deliveryNetwork.getLocationName(shortestPath[i]);
            if (i != shortestPath.size() - 1) {
                cout << " -> ";
            }
        }
        
        double totalDistance = deliveryNetwork.getPathDistance(shortestPath);
        cout << "\nTotal Distance: " << fixed << setprecision(1) << totalDistance << " km\n";
        
        // Show alternative routes (excluding the optimal one)
        deliveryNetwork.findAllRoutes(start, end, shortestPath, 3);
    }
    
    // Display the entire delivery network
    void displayNetwork() {
        deliveryNetwork.displayGraph();
    }
    
    // Plan delivery for multiple stops (TSP approximation - nearest neighbor)
    void planMultiStopDelivery(const string& pickUp, const vector<string>& stops) {
        cout << "\n========== MULTI-STOP DELIVERY PLAN ==========\n";
        cout << "Pick-Up: " << pickUp << "\n";
        cout << "Stops: ";
        for (const string& stop : stops) {
            cout << stop << " ";
        }
        cout << "\n\n";
        
        vector<string> remainingStops = stops;
        vector<vector<int>> fullRoutePaths;  // Store full paths between stops
        vector<string> stopSequence;        // Store the sequence of stops
        string currentLocation = pickUp;
        double totalDistance = 0;
        
        stopSequence.push_back(currentLocation);
        
        while (!remainingStops.empty()) {
            string nearestStop;
            double minDistance = numeric_limits<double>::max();
            vector<int> bestPath;
            
            // Find the nearest stop
            for (const string& stop : remainingStops) {
                vector<int> path = deliveryNetwork.findShortestPath(currentLocation, stop);
                double distance = deliveryNetwork.getPathDistance(path);
                
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestStop = stop;
                    bestPath = path;
                }
            }
            
            // Store the path to the nearest stop
            if (!bestPath.empty()) {
                fullRoutePaths.push_back(bestPath);
            }
            
            // Add nearest stop to sequence
            stopSequence.push_back(nearestStop);
            totalDistance += minDistance;
            
            // Remove from remaining stops
            remainingStops.erase(
                remove(remainingStops.begin(), remainingStops.end(), nearestStop),
                remainingStops.end()
            );
            
            currentLocation = nearestStop;
        }
        
        // Display the full optimized route with all paths
        cout << "OPTIMIZED DELIVERY ROUTE:\n";
        
        // Display each segment of the route
        for (size_t i = 0; i < fullRoutePaths.size(); i++) {
            cout << "\nSegment " << (i+1) << " (" << stopSequence[i] << " to " << stopSequence[i+1] << "):\n";
            
            // Display the path for this segment
            vector<int> path = fullRoutePaths[i];
            for (size_t j = 0; j < path.size(); j++) {
                cout << deliveryNetwork.getLocationName(path[j]);
                if (j != path.size() - 1) {
                    cout << " -> ";
                }
            }
            
            double segmentDistance = deliveryNetwork.getPathDistance(path);
            cout << "\nDistance: " << fixed << setprecision(1) << segmentDistance << " km\n";
        }
        
        cout << "\nTOTAL ROUTE DISTANCE: " << fixed << setprecision(1) << totalDistance << " km\n";
    }
};

#endif