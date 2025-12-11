#ifndef deliveryRoute_hpp
#define deliveryRoute_hpp

#include <iomanip>

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
        cout << "Estimated Delivery Time: " << fixed << setprecision(0) << (totalDistance * 2.5) << " minutes\n";
        
        // Show alternative routes
        deliveryNetwork.findAllRoutes(start, end, 3);
    }
    
    // Display the entire delivery network
    void displayNetwork() {
        deliveryNetwork.displayGraph();
    }
    
    // Plan delivery for multiple stops (TSP approximation - nearest neighbor)
    void planMultiStopDelivery(const string& depot, const vector<string>& stops) {
        cout << "\n========== MULTI-STOP DELIVERY PLAN ==========\n";
        cout << "Depot: " << depot << "\n";
        cout << "Stops: ";
        for (const string& stop : stops) {
            cout << stop << " ";
        }
        cout << "\n\n";
        
        vector<string> remainingStops = stops;
        vector<string> route;
        string currentLocation = depot;
        double totalDistance = 0;
        
        route.push_back(currentLocation);
        
        while (!remainingStops.empty()) {
            string nearestStop;
            double minDistance = numeric_limits<double>::max();
            
            for (const string& stop : remainingStops) {
                vector<int> path = deliveryNetwork.findShortestPath(currentLocation, stop);
                double distance = deliveryNetwork.getPathDistance(path);
                
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestStop = stop;
                }
            }
            
            // Add nearest stop to route
            route.push_back(nearestStop);
            totalDistance += minDistance;
            
            // Remove from remaining stops
            remainingStops.erase(
                remove(remainingStops.begin(), remainingStops.end(), nearestStop),
                remainingStops.end()
            );
            
            currentLocation = nearestStop;
        }
        
        // Return to depot
        vector<int> pathToDepot = deliveryNetwork.findShortestPath(currentLocation, depot);
        totalDistance += deliveryNetwork.getPathDistance(pathToDepot);
        route.push_back(depot);
        
        // Display the route
        cout << "OPTIMIZED DELIVERY ROUTE (Nearest Neighbor):\n";
        for (size_t i = 0; i < route.size(); i++) {
            cout << route[i];
            if (i != route.size() - 1) {
                cout << " -> ";
            }
        }
        cout << "\nTotal Route Distance: " << fixed << setprecision(1) << totalDistance << " km\n";
        cout << "Estimated Total Time: " << fixed << setprecision(0) << (totalDistance * 2.5) << " minutes\n";
    }
};

#endif