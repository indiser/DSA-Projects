#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>
#include "Graph.cpp"
using namespace std;

class RecommendationEngine
{
private:
    const Graph &graph;

public:
    RecommendationEngine(const Graph &graph) : graph(graph) {}

    vector<int> recommend_friends(int user_id)
    {
        vector<int> candidates;
        queue<pair<int, int>> q;
        unordered_set<int> visited;

        const vector<int> &direct_friends_vec = graph.get_friends(user_id);
        unordered_set<int> direct_friends_set(direct_friends_vec.begin(), direct_friends_vec.end());
        direct_friends_set.insert(user_id);

        q.push({user_id, 0});
        visited.insert(user_id);

        while (!q.empty())
        {
            pair<int, int> p = q.front();
            int current_user = p.first;
            int current_distance = p.second;
            q.pop();

            if (current_distance >= 2)
            {
                if (direct_friends_set.find(current_user) == direct_friends_set.end())
                {
                    candidates.push_back(current_user);
                }
                if (current_distance > 2)
                    continue;
            }

            for (int friend_id : graph.get_friends(current_user))
            {
                if (visited.find(friend_id) == visited.end())
                {
                    visited.insert(friend_id);
                    q.push({friend_id, current_distance + 1});
                }
            }
        }

        vector<pair<int, int>> ranked_recommendations;
        for (int candidate_id : candidates)
        {
            int mutual_friends = 0;
            const vector<int> &candidate_friends = graph.get_friends(candidate_id);
            for (int friends_id : candidate_friends)
            {
                if (direct_friends_set.count(friends_id) && friends_id != user_id)
                {
                    mutual_friends++;
                }
            }
            ranked_recommendations.push_back({mutual_friends, candidate_id});
        }

        sort(ranked_recommendations.begin(), ranked_recommendations.end(),
             [](const auto &a, const auto &b)
             {
                 return a.first > b.first;
             });

        vector<int> final_ranked_list;
        for (const auto &pair : ranked_recommendations)
        {
            final_ranked_list.push_back(pair.second);
        }

        return final_ranked_list;
    }
};

int main()
{
    Graph graph;

    string filename;
    cout << "Enter the name of the file: " << endl;
    getline(cin, filename);
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        int u, v;
        ss >> u >> v;
        graph.add_edge(u, v);
    }
    cout << "Graph loaded successfully!" << endl;

    RecommendationEngine engine(graph);
    int test_user;
    cout << "Enter The User: " << endl;
    cin >> test_user;
    if (!graph.user_exists(test_user))
    {
        cerr << "Error: User " << test_user << " does not exist in the graph." << endl;
    }

    vector<int> recommendations = engine.recommend_friends(test_user);
    cout << "\nRecommendations for User " << test_user << ":" << endl;
    for (int rec_id : recommendations)
    {
        cout << rec_id << " ";
    }
    cout << endl;
    return 0;
}