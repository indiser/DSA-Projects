// Date: 30/06/2026
#include<iostream>
#include<vector>
#include<unordered_map>
#include<queue>
using namespace std;


// Time Complexity: O(nlogk)
// Space Complexity: O(n)
class Solution
{
    public:
        vector<int> topKFreq(vector<int> nums, int k)
        {
            unordered_map<int, int> map;
            for(int val: nums) map[val]++;

            priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> p;

            for(auto &[num, count] : map)
            {
                p.push({count, num});

                if(p.size() > k)
                {
                    p.pop();
                }
            }

            vector<int> ans;
            while(!p.empty())
            {
                ans.push_back(p.top().second);
                p.pop();
            }

            return ans;
        }
};

int main()
{
    vector<int> nums ={1, 2, 1, 2, 1, 2, 3, 1, 3, 2};
    int k = 2;

    Solution s;
    vector<int> ans = s.topKFreq(nums, k);

    for(int val: ans)
    {
        cout<< val <<" ";
    }
    return 0;
}