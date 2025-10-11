#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <queue>
#include <limits>
#include <string>
#include<windows.h>
#include<cmath>
#include<algorithm>
using namespace std;

#define M_PI 3.14159265358979323846
const int INF = numeric_limits<int>::max();


vector<string> split(string &s, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}


class Dikstra
{
public:
    vector<vector<pair<int, int>>> loadForDikstra()
    {
        string filename = "routes.txt";
        ifstream file(filename);

        if (!file.is_open())
        {
            cout << "Error Occured!!" << endl;
            return {};
        }
        
        vector<pair<pair<int,int>, int>> edges;
        int maxNode = 0;
        string route;

        while (getline(file, route))
        {
            if (route.empty() || route[0] == '#')
                continue;
            auto tokens = split(route, ',');
            if (tokens.size() == 3)
            {
                try
                {
                    int u = stoi(tokens[0]);
                    int v = stoi(tokens[1]);
                    int weight = stoi(tokens[2]);
                    edges.push_back({{u, v}, weight});
                    maxNode = max(maxNode, max(u, v));
                }
                catch (const exception &e)
                {
                    cerr << e.what() << '\n';
                }
            }
        }
        
        vector<vector<pair<int, int>>> adj(maxNode + 1);
        for (auto &edge : edges)
        {
            int u = edge.first.first;
            int v = edge.first.second;
            int weight = edge.second;
            adj[u].push_back({v, weight});
            adj[v].push_back({u, weight});
        }
        
        return adj;
    }
    void find_path_using_Dikstra(int src)
    {
        vector<vector<pair<int, int>>> adj = loadForDikstra();
        int V = adj.size();
        vector<int> dist(V, INF);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty())
        {
            int u = pq.top().second;
            int d = pq.top().first;
            pq.pop();
            if (d > dist[u])
                continue;

            for (auto neighbour : adj[u])
            {
                int v = neighbour.first;
                int wt = neighbour.second;
                if (wt != INF && dist[u] + wt < dist[v])
                {
                    dist[v] = dist[u] + wt;
                    pq.push({dist[v], v});
                }
            }
        }

        cout << "\n========== DIJKSTRA'S ALGORITHM ==========\n";
        cout << "Source Node: " << src << "\n";
        cout << "==========================================\n";
        cout << "Node\tDistance(km)\n";
        cout << "----\t--------\n";
        for (int i = 0; i < dist.size(); i++)
        {
            if (dist[i] == INF)
                cout << i << "\tINF\n";
            else
                cout << i << "\t" << dist[i] << "\n";
        }
        cout << "==========================================\n\n";
    }
};

class AStarSearch
{
    public:
    struct Point
    {
        double latitude,longitude;
    };
    struct Location
    {
        int id;
        string name;
        double latitude, longitude;
    };
    
    unordered_map<int, Location> loadLocations() //h(n) value
    {
        string filename = "locations.txt";
        ifstream file(filename);
        unordered_map<int, Location> locations;

        if (!file.is_open())
        {
            cout << "Error loading locations!!" << endl;
            return {};
        }
        
        string line;
        while (getline(file, line))
        {
            if (line.empty() || line[0] == '#')
                continue;
            auto tokens = split(line, ',');
            if (tokens.size() == 4)
            {
                try
                {
                    Location loc;
                    loc.id = stoi(tokens[0]);
                    loc.name = tokens[1];
                    loc.latitude = stod(tokens[2]);
                    loc.longitude = stod(tokens[3]);
                    locations[loc.id] = loc;
                }
                catch (const exception &e)
                {
                    cerr << e.what() << '\n';
                }
            }
        }
        
        return locations;
    }
    
    vector<vector<pair<int, int>>> loadRoutes() //g(n) value
    {
        string filename = "routes.txt";
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error loading routes!!" << endl;
            return {};
        }
        
        vector<pair<pair<int,int>, int>> edges;
        int maxNode = 0;
        string route;
        while (getline(file, route)) {
            if (route.empty() || route[0] == '#') continue;
            auto tokens = split(route, ',');
            if (tokens.size() == 3) {
                int u = stoi(tokens[0]);
                int v = stoi(tokens[1]);
                int weight = stoi(tokens[2]);
                edges.push_back({{u, v}, weight});
                maxNode = max(maxNode, max(u, v));
            }
        }
        
        vector<vector<pair<int, int>>> adj(maxNode + 1);
        for (auto &edge : edges) {
            int u = edge.first.first;
            int v = edge.first.second;
            int weight = edge.second;
            adj[u].push_back({v, weight});
            adj[v].push_back({u, weight});
        }
        
        return adj;
    }
    
    double haversine(const Location& loc1, const Location& loc2) //Hurestic Function
    {
        double rad = 6371.0;
        double lat1 = loc1.latitude * M_PI / 180.0;
        double lat2 = loc2.latitude * M_PI / 180.0;
        double dLat = (loc2.latitude - loc1.latitude) * M_PI / 180.0;
        double dLon = (loc2.longitude - loc1.longitude) * M_PI / 180.0;
        
        double a = sin(dLat/2) * sin(dLat/2) + cos(lat1) * cos(lat2) * sin(dLon/2) * sin(dLon/2);
        double c = 2 * atan2(sqrt(a), sqrt(1-a));
        return rad * c;
    }

    int findCityByName(const string& cityName, const unordered_map<int, Location>& locations)
    {
        for (const auto& pair : locations) {
            if (pair.second.name == cityName) {
                return pair.second.id;
            }
        }
        return -1;
    }
    
    void find_path_using_AStar(int src, int dest)
    {
        unordered_map<int, Location> locations = loadLocations();
        vector<vector<pair<int, int>>> adj = loadRoutes();
        int n = adj.size();
        vector<double> g(n, INF);
        vector<double> f(n, INF);
        vector<int> parent(n, -1);
        vector<bool> closed(n, false);
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> openList;
        
        g[src] = 0.0;
        f[src] = haversine(locations[src], locations[dest]);
        openList.push({f[src], src});
        
        while (!openList.empty()) {
            int u = openList.top().second;
            openList.pop();
            
            if (closed[u]) continue;
            closed[u] = true;
            
            if (u == dest) {
                vector<int> path;
                int curr = dest;
                while (curr != -1) {
                    path.push_back(curr);
                    curr = parent[curr];
                }
                reverse(path.begin(), path.end());
                
                cout << "\n========== A* ALGORITHM ==========\n";
                cout << "Source: " << locations[src].name << " -> Destination: " << locations[dest].name << "\n";
                cout << "==================================\n";
                cout << "Path: ";
                for (int i = 0; i < path.size(); i++) {
                    cout << locations[path[i]].name;
                    if (i < path.size()-1) cout << " -> ";
                }
                cout << "\nTotal Distance: " << g[dest] << " km\n";
                cout << "Path Length: " << path.size() << " nodes\n";
                cout << "==================================\n\n";
                return;
            }
            
            for (auto edge : adj[u]) {
                int v = edge.first;
                int wt = edge.second;
                
                if (!closed[v]) {
                    double gNew = g[u] + wt;
                    double hNew = haversine(locations[v], locations[dest]);
                    double fNew = gNew + hNew;
                    
                    if (fNew < f[v]) {
                        g[v] = gNew;
                        f[v] = fNew;
                        parent[v] = u;
                        openList.push({fNew, v});
                    }
                }
            }
        }
        
        cout << "No path found from " << src << " to " << dest << "\n";
    }
};


class Solution
{
    public:
        void run()
        {
            while (true)
            {
                cout << "\n";
                cout << "╔══════════════════════════════════════════════════╗\n";
                cout << "║     🗺️  GRAPH PATHFINDING ALGORITHMS 🗺️         ║\n";
                cout << "╠══════════════════════════════════════════════════╣\n";
                cout << "║  [1] ⚡ Dijkstra's Algorithm                     ║\n";
                cout << "║  [2] ⭐ A* Heuristic Search                      ║\n";
                cout << "║  [0] 🚪 Exit                                     ║\n";
                cout << "╚══════════════════════════════════════════════════╝\n";
                cout << "\n👉 Enter your choice: ";
                
                int ch;
                cin>>ch;
                
                if(ch == 0) {
                    cout << "\n✅ Thank you for using the pathfinding tool!\n";
                    break;
                }

                switch (ch)
                {
                case 1:
                    {
                        int src;
                        cout << "\n📍 Enter source node : ";
                        cin >> src;
                        Dikstra d;
                        d.find_path_using_Dikstra(src);
                    }
                    break;
                case 2:
                    {
                        cin.ignore();
                        string srcName, destName;
                        cout << "\n📍 Enter source city name: ";
                        getline(cin, srcName);
                        cout << "🎯 Enter destination city name: ";
                        getline(cin, destName);
                        
                        AStarSearch a;
                        auto locations = a.loadLocations();
                        int src = a.findCityByName(srcName, locations);
                        int dest = a.findCityByName(destName, locations);
                        
                        if (src == -1) {
                            cout << "❌ Source city '" << srcName << "' not found!\n";
                        } else if (dest == -1) {
                            cout << "❌ Destination city '" << destName << "' not found!\n";
                        } else {
                            a.find_path_using_AStar(src, dest);
                        }
                    }
                    break;
                default:
                    cout << "\n❌ Invalid choice! Please select 0-4.\n";
                    return;
                }
                
                cout << "\nPress Enter to continue...";
                cin.get();
            }
            
        }
};


int main()
{
    SetConsoleOutputCP(65001);
    Solution s;
    s.run();
}