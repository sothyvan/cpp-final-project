#include "graph.hpp"
#include "deliveryRoute.hpp"

using namespace std;

int main() {
    cout << "=========================================\n";
    cout << "   DELIVERY ROUTE OPTIMIZATION SYSTEM   \n";
    cout << "=========================================\n";
    
    DeliveryRouteOptimizer optimizer;
    
    // Initialize with sample data
    optimizer.initializeSampleNetwork();
    
    int choice;
    do {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Display Delivery Network\n";
        cout << "2. Optimize Single Route\n";
        cout << "3. Plan Multi-Stop Delivery\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: {
                optimizer.displayNetwork();
                break;
            }
            
            case 2: {
                string start, end;
                cout << "Enter starting location: ";
                cin.ignore();
                getline(cin, start);
                cout << "Enter destination: ";
                getline(cin, end);
                optimizer.optimizeRoute(start, end);
                break;
            }
            
            case 3: {
                string pickUp;
                int numStops;
                cout << "Enter pick-up location: ";
                cin.ignore();
                getline(cin, pickUp);
                cout << "Enter number of delivery stops: ";
                cin >> numStops;
                
                vector<string> stops;
                cin.ignore();
                for (int i = 0; i < numStops; i++) {
                    string stop;
                    cout << "Enter stop " << (i+1) << ": ";
                    getline(cin, stop);
                    stops.push_back(stop);
                }
                
                optimizer.planMultiStopDelivery(pickUp, stops);
                break;
            }
            
            case 0: {
                cout << "Exiting Delivery Route Optimization System.\n";
                break;
            }
            
            default: {
                cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
        
    } while (choice != 0);
    
    return 0;
}