#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;


// Time Complexity: add O(n), remove O(n), contains O(n)
// Space Complexity: O(n) where n is number of elements
class MyHashSet
{
    public:
        vector<int> vec;
        MyHashSet()
        {

        }
        void add(int key)
        {
            for(int val:vec)
            {
                if(val==key)
                {
                    return;
                }
            }
            vec.push_back(key);
        }
        void remove(int key)
        {
            for(int i=0; i<vec.size(); i++)
            {
                if(vec[i]==key)
                {
                    vec.erase(vec.begin()+i);
                    return;
                }
            }
        }
        bool contains(int key)
        {
            for(int val:vec)
            {
                if(val==key)
                {
                    return true;
                }
            }
            return false;
        }
};

// Time Complexity: add O(1), remove O(1), contains O(1) average case
// Space Complexity: O(n) where n is number of elements
class MyHashSetOptimized
{
    public:
        unordered_map<int, bool> map;
        MyHashSetOptimized()
        {
        }
        void add(int key)
        {
            map[key] = true;
        }
        void remove(int key)
        {
            map.erase(key);
        }
        bool contains(int key)
        {
            return map.find(key) != map.end();
        }
};

int main()
{
    MyHashSet *set= new MyHashSet();
    set->add(1);
    set->add(2);
    cout<<set->contains(2)<<endl;
    cout<<set->contains(3)<<endl;
    set->add(2);
    cout<<set->contains(2)<<endl;
    set->remove(2);
    cout<<set->contains(2)<<endl;
    delete set;
    return 0;
}