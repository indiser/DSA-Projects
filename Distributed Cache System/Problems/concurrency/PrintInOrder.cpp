#include<iostream>
#include<vector>
#include<unordered_map>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<bits/stdc++.h>
using namespace std;



// Time Complexity: O(1) for each method
// Space Complexity: O(1) - only stores one integer and synchronization primitives
class Foo {
    private:
        mutex mtx;
        condition_variable cv;
        int order = 0;
        
    public:
        Foo() {}

        void first(function<void()> printFirst) {
            unique_lock<mutex> lock(mtx);
            printFirst();
            order = 1;
            cv.notify_all();
        }

        void second(function<void()> printSecond) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this] { return order == 1; });
            printSecond();
            order = 2;
            cv.notify_all();
        }

        void third(function<void()> printThird) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this] { return order == 2; });
            printThird();
        }
};

int main()
{
    Foo foo;
    thread t1([&]() { foo.first([]() { cout << "first"; }); });
    thread t2([&]() { foo.second([]() { cout << "second"; }); });
    thread t3([&]() { foo.third([]() { cout << "third"; }); });
    
    t3.join();
    t2.join();
    t1.join();
    
    cout << endl;
    return 0;
}