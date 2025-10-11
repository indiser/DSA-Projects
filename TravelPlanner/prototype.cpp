#include<iostream>
#include<vector>
#include<unordered_map>
#include<queue>
#include<cmath>
#include<limits>
#include<climits>
#include<stack>
#include<numeric>
#include<string>
#include<algorithm>
#include<Windows.h>
#include<iomanip>
using namespace std;

vector<vector<pair<int,int>>> graph = {
    {{1,5},{2,3},{3,8}},
    {{0,5},{4,7},{5,2}},
    {{0,3},{7,4},{8,6}},
    {{0,8},{9,1},{10,5}},
    {{1,7},{11,3},{12,8}},
    {{1,2},{13,6},{14,4}},
    {{1,9},{15,7},{16,2}},
    {{2,4},{17,5},{18,9}},
    {{2,6},{19,3},{20,7}},
    {{3,1},{21,8},{22,4}},
    {{3,5},{23,2},{24,6}},
    {{4,3},{25,9},{26,1}},
    {{4,8},{27,5},{28,7}},
    {{5,6},{29,3},{30,8}},
    {{5,4},{31,2},{32,9}},
    {{6,7},{33,4},{34,6}},
    {{6,2},{35,8},{36,3}},
    {{7,5},{37,1},{38,7}},
    {{7,9},{39,5},{40,2}},
    {{8,3},{41,6},{42,8}},
    {{8,7},{43,4},{44,9}},
    {{9,8},{45,2},{46,5}},
    {{9,4},{47,7},{48,3}},
    {{10,2},{49,6},{50,1}},
    {{10,6},{51,8},{52,4}},
    {{11,9},{53,3},{54,7}},
    {{11,1},{55,5},{56,2}},
    {{12,5},{57,8},{58,6}},
    {{12,7},{59,4},{60,9}},
    {{13,3},{61,1},{62,5}},
    {{13,8},{63,7},{64,2}},
    {{14,2},{65,6},{66,4}},
    {{14,9},{67,3},{68,8}},
    {{15,4},{69,5},{70,1}},
    {{15,6},{71,7},{72,9}},
    {{16,8},{73,2},{74,4}},
    {{16,3},{75,6},{76,5}},
    {{17,1},{77,8},{78,7}},
    {{17,7},{79,3},{80,2}},
    {{18,5},{81,9},{82,4}},
    {{18,2},{83,6},{84,1}},
    {{19,6},{85,8},{86,5}},
    {{19,8},{87,3},{88,7}},
    {{20,4},{89,2},{90,9}},
    {{20,9},{91,1},{92,6}},
    {{21,2},{93,5},{94,8}},
    {{21,5},{95,4},{96,3}},
    {{22,8},{97,7},{98,2}},
    {{22,3},{99,6},{0,9}},
    {{23,7},{1,4},{2,1}}
};

const int INF=INT_MAX;

class Solution
{
    private:
        vector<vector<int>> adjList(vector<vector<pair<int,int>>> &graph)
        {
            int n=graph.size();
            vector<vector<int>> adj(n, vector<int>(n,INF));

            for (int i = 0; i < n; i++)
            {
                adj[i][i]=0;
            }
            
            for(int i = 0; i < graph.size(); i++)
            {
                for(auto edge : graph[i])
                {
                    int v = edge.first;
                    int wt = edge.second;
                    adj[i][v]=wt;
                }
            }
            return adj;
        }
        
        void dikstra(vector<vector<pair<int,int>>> &graph, int src)
        {
            vector<vector<int>> adj=adjList(graph);
            int n=graph.size();
            vector<int> dist(n,INF);
            priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
            dist[src]=0;
            pq.push({0,src});

            while(!pq.empty())
            {
                int u=pq.top().second;
                int d=pq.top().first;
                pq.pop();

                if(d > dist[u]) continue;

                for(int v = 0; v < n; v++)
                {
                    int wt = adj[u][v];

                    if(wt != INF && dist[u]+wt<dist[v])
                    {
                        dist[v]=dist[u]+wt;
                        pq.push({dist[v],v});
                    }
                }
            }

            cout << "\n========== DIJKSTRA'S ALGORITHM ==========\n";
            cout << "Source Node: " << src << "\n";
            cout << "==========================================\n";
            cout << "Node\tDistance\n";
            cout << "----\t--------\n";
            for(int i = 0; i < dist.size(); i++)
            {
                if(dist[i] == INF)
                    cout << i << "\tINF\n";
                else
                    cout << i << "\t" << dist[i] << "\n";
            }
            cout << "==========================================\n\n";
        }
        void AStarSearch(vector<vector<pair<int,int>>> &graph, int src, int dest)
        {
            int n=graph.size();
            if(src < 0 || src >= n || dest < 0 || dest >= n)
            {
                cout<<"Source or Destination is not valid"<<endl;
                return;
            }
            if(src == dest)
            {
                cout<<"Already at destination"<<endl;
                return;
            }
            
            vector<double> g(n, INF);
            vector<double> f(n, INF);
            vector<int> parent(n, -1);
            vector<bool> closed(n, false);
            priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int>>> openList;
            
            g[src] = 0.0;
            f[src] = 0.0;
            openList.push({0.0, src});
            
            while(!openList.empty())
            {
                int u = openList.top().second;
                openList.pop();
                
                if(closed[u]) continue;
                closed[u] = true;
                
                if(u == dest)
                {
                    vector<int> path;
                    int curr = dest;
                    while(curr != -1)
                    {
                        path.push_back(curr);
                        curr = parent[curr];
                    }
                    reverse(path.begin(), path.end());
                    
                    cout << "\n========== A* ALGORITHM ==========\n";
                    cout << "Source: " << src << " -> Destination: " << dest << "\n";
                    cout << "==================================\n";
                    cout << "Path: ";
                    for(int i = 0; i < path.size(); i++) {
                        cout << path[i];
                        if(i < path.size()-1) cout << " -> ";
                    }
                    cout << "\nTotal Distance: " << g[dest] << "\n";
                    cout << "Path Length: " << path.size() << " nodes\n";
                    cout << "==================================\n\n";
                    return;
                }
                
                for(auto edge : graph[u])
                {
                    int v = edge.first;
                    int wt = edge.second;
                    
                    if(!closed[v])
                    {
                        double gNew = g[u] + wt;
                        double hNew = 0.0;
                        double fNew = gNew + hNew;
                        
                        if(fNew < f[v])
                        {
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
        void bellmanFord(vector<vector<pair<int,int>>> &graph, int src)
        {
            int n=graph.size();
            vector<int> dist(n,INF);
            dist[src]=0;
            for (int i = 0; i < n-1; i++)
            {
                for (int u = 0; u < n; u++)
                {
                    for(auto edge:graph[u])
                    {
                        int v=edge.first;
                        int wt=edge.second;

                        if(dist[u] != INF && dist[u]+wt<dist[v])
                        {
                            dist[v]=dist[u]+wt;
                        }
                    }
                }
                
            }
            
            for (int u = 0; u < n; u++)
            {
                for(auto edge:graph[u])
                {
                    int v=edge.first;
                    int wt=edge.second;

                    if(dist[u] != INF && dist[u]+wt < dist[v])
                    {
                        cout<<"No Negetive Edges"<<endl;
                        return;
                    }
                }
            }
                
            cout << "\n========== BELLMAN-FORD ALGORITHM ==========\n";
            cout << "Source Node: " << src << "\n";
            cout << "============================================\n";
            cout << "Node\tDistance\n";
            cout << "----\t--------\n";
            for(int i = 0; i < dist.size(); i++)
            {
                if(dist[i] == INF)
                    cout << i << "\tINF\n";
                else
                    cout << i << "\t" << dist[i] << "\n";
            }
            cout << "============================================\n\n";
        }
        void floydWarshell(vector<vector<pair<int,int>>> &graph)
        {
            int n=graph.size();
            vector<vector<int>> dist=adjList(graph);

            for (int k = 0; k < n; k++)
            {
                for (int i = 0; i < n; i++)
                {
                    for (int j = 0; j < n; j++)
                    {
                        if(dist[i][k] != INF && dist[k][j] != INF)
                        {
                            dist[i][j]=min(dist[i][j],dist[i][k]+dist[k][j]);
                        }
                    }
                }
            }
            
            cout << "\n========== FLOYD-WARSHALL ALGORITHM ==========\n";
            cout << "All-Pairs Shortest Paths Matrix\n";
            cout << "==============================================\n";
            cout << "From\\To\t";
            for(int j = 0; j < min(n, 10); j++) cout << j << "\t";
            cout << "\n";
            for(int i = 0; i < min(n, 10); i++)
            {
                cout << i << "\t";
                for(int j = 0; j < min(n, 10); j++)
                {
                    if(dist[i][j] == INF)
                        cout << "INF\t";
                    else
                        cout << dist[i][j] << "\t";
                }
                cout << "\n";
            }
            if(n > 10) cout << "... (showing first 10x10 nodes only)\n";
            cout << "==============================================\n\n";
        }
    public:
        void run()
        {
            while(true)
            {
                cout << "\n";
                cout << "╔══════════════════════════════════════════════════╗\n";
                cout << "║     🗺️  GRAPH PATHFINDING ALGORITHMS 🗺️         ║\n";
                cout << "╠══════════════════════════════════════════════════╣\n";
                cout << "║  [1] ⚡ Dijkstra's Algorithm                     ║\n";
                cout << "║  [2] ⭐ A* Heuristic Search                      ║\n";
                cout << "║  [3] 🔄 Bellman-Ford Algorithm                  ║\n";
                cout << "║  [4] 🌐 Floyd-Warshall Algorithm                ║\n";
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
                        cout << "\n📍 Enter source node (0-" << graph.size()-1 << "): ";
                        cin >> src;
                        dikstra(graph, src);
                    }
                    break;
                case 2:
                    {
                        int src, dest;
                        cout << "\n📍 Enter source node (0-" << graph.size()-1 << "): ";
                        cin >> src;
                        cout << "🎯 Enter destination node (0-" << graph.size()-1 << "): ";
                        cin >> dest;
                        AStarSearch(graph, src, dest);
                    }
                    break;
                case 3:
                    {   
                        int src;
                        cout << "\n📍 Enter source node (0-" << graph.size()-1 << "): ";
                        cin >> src;
                        bellmanFord(graph,src);
                    }
                    break;
                case 4:
                    cout << "\n⏳ Computing all-pairs shortest paths...\n";
                    floydWarshell(graph);
                    break;
                default:
                    cout << "\n❌ Invalid choice! Please select 0-4.\n";
                    break;
                }
                
                cout << "\nPress Enter to continue...";
                cin.ignore();
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