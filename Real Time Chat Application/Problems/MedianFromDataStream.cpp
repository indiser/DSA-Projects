#include<iostream>
#include<vector>
#include<unordered_map>
#include<queue>
using namespace std;

class MedianFinder {
private:
    priority_queue<int> maxHeap; // Left half (smaller numbers)
    priority_queue<int, vector<int>, greater<int>> minHeap; // Right half (larger numbers)
    
public:
    MedianFinder() {
        // Constructor - heaps are automatically initialized
    }
    
    void addNum(int num) {
        // Add to maxHeap first
        maxHeap.push(num);
        
        // Move the largest from maxHeap to minHeap
        minHeap.push(maxHeap.top());
        maxHeap.pop();
        
        // Balance the heaps - maxHeap should have equal or one more element
        if(maxHeap.size() < minHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }
    
    double findMedian() {
        if(maxHeap.size() > minHeap.size()) {
            return maxHeap.top();
        } else {
            return (maxHeap.top() + minHeap.top()) / 2.0;
        }
    }
};

int main()
{
    MedianFinder mf;

    mf.addNum(1);
    cout << "Median: " << mf.findMedian() << endl;   // 1

    mf.addNum(2);
    cout << "Median: " << mf.findMedian() << endl;   // 1.5

    mf.addNum(3);
    cout << "Median: " << mf.findMedian() << endl;   // 2

    mf.addNum(4);
    cout << "Median: " << mf.findMedian() << endl;   // 2.5

    mf.addNum(5);
    cout << "Median: " << mf.findMedian() << endl;   // 3

    return 0;
}