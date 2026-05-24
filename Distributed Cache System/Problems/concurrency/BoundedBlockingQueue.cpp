#include<iostream>
#include<vector>
#include<unordered_map>
#include<thread>
#include<condition_variable>
#include<mutex>
#include<deque>
using namespace std;


// Time Complexity: enqueue O(1), dequeue O(1), size O(1)
// Space Complexity: O(capacity) for storing elements
class BoundedBlockingQueue
{
    private:
        deque<int> queue;
        int capacity;
        mutex mtx;
        condition_variable notFull, notEmpty;
        
    public:
        BoundedBlockingQueue(int capacity) : capacity(capacity) {}
        
        void enqueue(int x)
        {
            unique_lock<mutex> lock(mtx);
            notFull.wait(lock, [this] { return queue.size() < capacity; });
            queue.push_back(x);
            notEmpty.notify_one();
        }
        
        int dequeue()
        {
            unique_lock<mutex> lock(mtx);
            notEmpty.wait(lock, [this] { return !queue.empty(); });
            int val = queue.front();
            queue.pop_front();
            notFull.notify_one();
            return val;
        }
        
        int size()
        {
            unique_lock<mutex> lock(mtx);
            return queue.size();
        }
};

int main()
{
    BoundedBlockingQueue q(3);
    
    thread producer([&q]() {
        for (int i = 1; i <= 5; i++) {
            q.enqueue(i);
            cout << "Produced: " << i << endl;
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    });
    
    thread consumer([&q]() {
        for (int i = 0; i < 5; i++) {
            int val = q.dequeue();
            cout << "Consumed: " << val << endl;
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    });
    
    producer.join();
    consumer.join();
    
    return 0;
}