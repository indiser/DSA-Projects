#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;

/*
Time Complexity: O(1) for both get() and put() operations
Space Complexity: O(capacity) for storing nodes in hashmap and doubly linked list
*/

class LRUCache
{
    public:
        class Node
        {
            public:
                int key,val;
                Node *prev;
                Node *next;

                Node(int _key,int _val)
                {
                    key=_key;
                    val=_val;
                    prev=next=nullptr;
                }
        };

        Node *head = new Node(-1,-1);
        Node *tail = new Node(-1, -1);

        unordered_map<int, Node*> mp;
        int limit;

        void addNode(Node *newNode)
        {
            Node *oldNode=head->next;
            head->next=newNode;
            oldNode->prev=newNode;
            newNode->next=oldNode;
            newNode->prev=head;
        }
        void delNode(Node *oldNode)
        {
            Node *oldPrev=oldNode->prev;
            Node *oldNext=oldNode->next;
            oldPrev->next=oldNext;
            oldNext->prev=oldPrev;
        }

        LRUCache(int capacity)
        {
            limit=capacity;
            head->next=tail;
            tail->prev=head;
        }

        int get(int key)
        {
            if(mp.find(key)==mp.end())
            {
                return -1;
            }
            Node *ansNode=mp[key];
            int ans=ansNode->val;
            mp.erase(key);
            delNode(ansNode);
            addNode(ansNode);
            mp[key]=ansNode;
            return ans;
        }

        void put(int key, int value)
        {
            if(mp.find(key) != mp.end())
            {
                Node *oldNode=mp[key];
                delNode(oldNode);
                mp.erase(key);
            }

            if(mp.size() == limit)
            {
                mp.erase(tail->prev->key);
                delNode(tail->prev);
            }

            Node *newNode = new Node(key, value);
            addNode(newNode);
            mp[key]=newNode;
        }

};

int main()
{
    LRUCache *lru=new LRUCache(2);
    lru->put(1,1);
    lru->put(2,2);
    cout << lru->get(1) << endl;
    lru->put(3,3);
    cout << lru->get(2) << endl;
    cout << lru->get(3) << endl; 
    cout << lru->get(1) << endl;
    return 0;
}