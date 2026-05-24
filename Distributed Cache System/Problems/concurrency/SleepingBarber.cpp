#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<chrono>
#include<bits/stdc++.h>
using namespace std;

// Time Complexity: O(1) per operation (constant time for mutex lock/unlock and condition variable operations)
// Space Complexity: O(n) where n is the number of waiting chairs (queue size)
class SleepingBarber {
private:
    int waitingChairs;
    queue<int> waitingCustomers;
    mutex mtx;
    condition_variable barberReady;
    condition_variable customerReady;
    bool barberSleeping;
    int currentCustomer;

public:
    SleepingBarber(int chairs) : waitingChairs(chairs), barberSleeping(true), currentCustomer(-1) {}

    void customerArrives(int customerId) {
        unique_lock<mutex> lock(mtx);
        
        // If barber is sleeping and no one waiting, wake barber
        if (barberSleeping && waitingCustomers.empty()) {
            barberSleeping = false;
            currentCustomer = customerId;
            cout << "Customer " << customerId << " wakes up barber\n";
            barberReady.notify_one();
        }
        // If waiting room has space, customer sits
        else if (waitingCustomers.size() < waitingChairs) {
            waitingCustomers.push(customerId);
            cout << "Customer " << customerId << " sits in waiting room (waiting: " 
                 << waitingCustomers.size() << ")\n";
            customerReady.notify_one();
        }
        // No space, customer leaves
        else {
            cout << "Customer " << customerId << " leaves (no chairs available)\n";
        }
    }

    void barberWork() {
        while (true) {
            unique_lock<mutex> lock(mtx);
            
            // If no customers, barber sleeps
            if (waitingCustomers.empty() && currentCustomer == -1) {
                barberSleeping = true;
                cout << "Barber goes to sleep\n";
                barberReady.wait(lock);
                continue;
            }
            
            // Get next customer from queue
            if (!waitingCustomers.empty()) {
                currentCustomer = waitingCustomers.front();
                waitingCustomers.pop();
            }
            
            int customer = currentCustomer;
            lock.unlock();
            
            // Cut hair (simulate work)
            cout << "Barber cutting hair for customer " << customer << "\n";
            this_thread::sleep_for(chrono::milliseconds(100));
            
            lock.lock();
            cout << "Barber finished with customer " << customer << "\n";
            currentCustomer = -1;
        }
    }
};

int main() {
    SleepingBarber barber(3); // 3 waiting chairs
    
    thread barberThread([&barber]() {
        barber.barberWork();
    });
    barberThread.detach();
    
    // Simulate customers arriving
    for (int i = 1; i <= 8; i++) {
        barber.customerArrives(i);
        this_thread::sleep_for(chrono::milliseconds(50));
    }
    
    this_thread::sleep_for(chrono::seconds(2));
    cout << "Simulation complete\n";
    
    return 0;
}
