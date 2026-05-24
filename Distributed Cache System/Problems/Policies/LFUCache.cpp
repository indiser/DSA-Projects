#include <iostream>
#include <unordered_map>
using namespace std;

class Node {
public:
    int key;
    int value;
    int cnt;
    Node* next;
    Node* prev;

    Node(int key, int val) : key(key), value(val), cnt(1), next(nullptr), prev(nullptr) {}
};

class LFUCache {
public:
    unordered_map<int, Node*> cacheMap;
    unordered_map<int, pair<Node*, Node*>> freqMap;
    int minFreq;
    int capacity;

    LFUCache(int capacity) : minFreq(0), capacity(capacity) {}

    ~LFUCache() {
        for (auto& p : cacheMap) {
            delete p.second;
        }
        for (auto& p : freqMap) {
            delete p.second.first;
            delete p.second.second;
        }
    }

    void add(Node* node, int freq) {
        if (freqMap.find(freq) == freqMap.end()) {
            Node* head = new Node(-1, -1);
            Node* tail = new Node(-1, -1);
            head->next = tail;
            tail->prev = head;
            freqMap[freq] = {head, tail};
        }

        Node* head = freqMap[freq].first;
        Node* temp = head->next;
        node->next = temp;
        node->prev = head;
        head->next = node;
        temp->prev = node;
    }

    void remove(Node* node) {
        Node* delPrev = node->prev;
        Node* delNext = node->next;
        delPrev->next = delNext;
        delNext->prev = delPrev;
    }

    void updateFreq(Node* node) {
        int oldFreq = node->cnt;
        node->cnt++;
        remove(node);
        if (freqMap[oldFreq].first->next == freqMap[oldFreq].second) {
            freqMap.erase(oldFreq);
            if (minFreq == oldFreq) {
                minFreq++;
            }
        }
        add(node, node->cnt);
    }

    int get(int key) {
        if (cacheMap.find(key) == cacheMap.end()) {
            return -1;
        }
        Node* node = cacheMap[key];
        int res = node->value;
        updateFreq(node);
        return res;
    }

    void put(int key, int value) {
        if (capacity == 0) {
            return;
        }
        if (cacheMap.find(key) != cacheMap.end()) {
            Node* node = cacheMap[key];
            node->value = value;
            updateFreq(node);
        } else {
            if ((int)cacheMap.size() == capacity) {
                Node* node = freqMap[minFreq].second->prev;
                cacheMap.erase(node->key);
                remove(node);

                if (freqMap[minFreq].first->next == freqMap[minFreq].second) {
                    freqMap.erase(minFreq);
                }

                delete node;
            }
            Node* node = new Node(key, value);
            cacheMap[key] = node;
            minFreq = 1;
            add(node, 1);
        }
    }
};

int main() {
    LFUCache cache1(2);
    cache1.put(1, 1);
    cache1.put(2, 2);
    cout << "Example 1 - Basic Operations:" << endl;
    cout << "get(1): " << cache1.get(1) << endl;
    cache1.put(3, 3);
    cout << "get(2): " << cache1.get(2) << endl;
    cout << endl;

    LFUCache cache2(3);
    cache2.put(1, 10);
    cache2.put(2, 20);
    cache2.put(3, 30);
    cout << "Example 2 - Frequency Tracking:" << endl;
    cout << "get(1): " << cache2.get(1) << endl;
    cout << "get(1): " << cache2.get(1) << endl;
    cache2.put(4, 40);
    cout << "get(2): " << cache2.get(2) << endl;
    cout << endl;

    LFUCache cache3(2);
    cache3.put(1, 100);
    cache3.put(2, 200);
    cout << "Example 3 - LRU Tie-breaking:" << endl;
    cache3.put(3, 300);
    cout << "get(1): " << cache3.get(1) << endl;
    cout << "get(2): " << cache3.get(2) << endl;
    cout << "get(3): " << cache3.get(3) << endl;

    return 0;
}
