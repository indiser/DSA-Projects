#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

class HitCounter
{
    public:
        vector<int> times;
        HitCounter() {};
        void hit(int timestamp)
        {
            times.push_back(timestamp);
        }
        int getHits(int timestamp)
        {
            return times.end() - lower_bound(times.begin(), times.end(), timestamp - 300 + 1);
        }
};

int main()
{
    HitCounter counter;
    counter.hit(1);
    counter.hit(2);
    counter.hit(3);
    cout << counter.getHits(4) << endl;   // 3
    counter.hit(300);
    cout << counter.getHits(300) << endl; // 4
    cout << counter.getHits(301) << endl; // 3
    return 0;
}