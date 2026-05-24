#include<iostream>
#include<vector>
#include<thread>
#include<mutex>
#include<algorithm>
#include<bits/stdc++.h>
using namespace std;


// Time Complexity: O(1) per operation (constant time for mutex lock/unlock)
// Space Complexity: O(1) - fixed 5 mutexes regardless of input
class DiningPhilosophers {
private:
    mutex forks[5];
    
public:
    DiningPhilosophers() {}

    void wantsToEat(int philosopher,
                    function<void()> pickLeftFork,
                    function<void()> pickRightFork,
                    function<void()> eat,
                    function<void()> putLeftFork,
                    function<void()> putRightFork) {
        int left = philosopher;
        int right = (philosopher + 1) % 5;
        
        // Acquire forks in order (left < right) to prevent circular wait deadlock
        unique_lock<mutex> lock1(forks[min(left, right)]);
        unique_lock<mutex> lock2(forks[max(left, right)]); 
        
        pickLeftFork();
        pickRightFork();
        eat();
        putLeftFork();
        putRightFork();
    }
};

int main() {
    DiningPhilosophers dp;
    vector<thread> threads;
    
    for (int i = 0; i < 5; i++) {
        threads.emplace_back([&dp, i]() {
            for (int j = 0; j < 3; j++) {
                dp.wantsToEat(i,
                    [i]() { cout << "Philosopher " << i << " picks left fork\n"; },
                    [i]() { cout << "Philosopher " << i << " picks right fork\n"; },
                    [i]() { cout << "Philosopher " << i << " is eating\n"; },
                    [i]() { cout << "Philosopher " << i << " puts left fork\n"; },
                    [i]() { cout << "Philosopher " << i << " puts right fork\n"; }
                );
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    cout << "All philosophers finished eating!\n";
    return 0;
}
