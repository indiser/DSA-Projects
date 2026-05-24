#include<iostream>
#include<vector>
#include<unordered_map>
#include<random>
#include<iterator>
using namespace std;



// Time Complexity: insert O(1), remove O(1), getRandom O(1) avg
// Space Complexity: O(n) where n is number of elements
class RandomizedSet {
    public:
        unordered_map<int, bool> map;
        RandomizedSet() 
        {
        
        }
    
        bool insert(int val) 
        {
            if(map.find(val) != map.end()) return false;
            map[val]=true;
            return true;
        }
    
        bool remove(int val) 
        {
            return map.erase(val) > 0;
        }
    
        int getRandom() 
        {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dist(0, map.size()-1);
            auto it=map.begin();
            advance(it, dist(gen));
            return it->first;
        }
};


int main()
{
    RandomizedSet *rset = new RandomizedSet();
    cout<<rset->insert(1)<<endl;
    cout<<rset->insert(2)<<endl;
    cout<<rset->insert(3)<<endl;
    cout<<rset->insert(4)<<endl;
    cout<<rset->insert(5)<<endl;
    cout<<rset->remove(6)<<endl;
    cout<<rset->getRandom()<<endl;
    return 0;
}