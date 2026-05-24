#include<iostream>
#include<vector>
using namespace std;

// Time Complexity: put() O(1) avg, get() O(1) avg, remove() O(1) avg
// Space Complexity: O(n) where n is number of key-value pairs
class MyHashMap {
    private:
        static const int SIZE = 10000;
        vector<vector<pair<int,int>>> buckets;
        
        int hash(int key) {
            return key % SIZE;
        }
    
    public:
        MyHashMap() : buckets(SIZE) {}
    
        void put(int key, int value) {
            int idx = hash(key);
            for(auto &p : buckets[idx]) {
                if(p.first == key) {
                    p.second = value;
                    return;
                }
            }
            buckets[idx].push_back({key, value});
        }
    
        int get(int key) {
            int idx = hash(key);
            for(auto &p : buckets[idx]) {
                if(p.first == key) {
                    return p.second;
                }
            }
            return -1;
        }
    
        void remove(int key) {
            int idx = hash(key);
            for(int i = 0; i < buckets[idx].size(); i++) {
                if(buckets[idx][i].first == key) {
                    buckets[idx].erase(buckets[idx].begin() + i);
                    return;
                }
            }
        }
};

int main()
{
    MyHashMap map;
    map.put(1, 1);
    cout << map.get(1) << endl;
    cout << map.get(2) << endl;
    map.put(2, 2);
    cout << map.get(2) << endl;
    map.remove(2);
    cout << map.get(2) << endl;
    return 0;
}
