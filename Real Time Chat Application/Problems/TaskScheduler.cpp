#include<iostream>
#include<vector>
#include<unordered_map>
#include<queue>
using namespace std;

class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        priority_queue<pair<int, int>> pq;
        vector<int> cnt(26, 0);

        for (auto& it : tasks)
            cnt[it - 'A']++;

        for (int i = 0; i < 26; i++) {
            if (cnt[i])
                pq.push({cnt[i], i});

            cnt[i] = n;
        }

        int ans = 0;

        while (!pq.empty()) {
            priority_queue<pair<int, int>> temp;
            int fl = 1;

            while (!pq.empty()) {
                int c = pq.top().first, t = pq.top().second;
                pq.pop();

                if (cnt[t] >= n) {
                    if (c > 1)
                        temp.push({c - 1, t});

                    fl = 0;

                    for (int i = 0; i < 26; i++)
                        cnt[i]++;

                    cnt[t] = 0;
                    break;
                }

                temp.push({c, t});
            }

            if (fl)
                for (int i = 0; i < 26; i++)
                    cnt[i]++;

            while (!temp.empty()) {
                pq.push(temp.top());
                temp.pop();
            }

            ans++;
        }

        return ans;
    }
};

int main()
{
    vector<char> tasks = {'A', 'A', 'A', 'B', 'B', 'B'};
    int n = 2;
    Solution s;
    cout << s.leastInterval(tasks, n) <<endl;
    return 0;
}