#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<functional>
using namespace std;

class FizzBuzz {
    private:
        int n;
        int current;
        mutex mtx;
        condition_variable cv;

    public:
        FizzBuzz(int n) : n(n), current(1) {}

        void fizz(function<void()> printFizz) 
        {
            for(int i = 3; i <= n; i += 3) {
                if(i % 5 == 0) continue;
                unique_lock<mutex> lock(mtx);
                cv.wait(lock, [this, i]() { return current == i; });
                printFizz();
                current++;
                cv.notify_all();
            }
        }

        void buzz(function<void()> printBuzz) 
        {
            for(int i = 5; i <= n; i += 5) {
                if(i % 3 == 0) continue;
                unique_lock<mutex> lock(mtx);
                cv.wait(lock, [this, i]() { return current == i; });
                printBuzz();
                current++;
                cv.notify_all();
            }
        }

	    void fizzbuzz(function<void()> printFizzBuzz) 
        {
            for(int i = 15; i <= n; i += 15) {
                unique_lock<mutex> lock(mtx);
                cv.wait(lock, [this, i]() { return current == i; });
                printFizzBuzz();
                current++;
                cv.notify_all();
            }
        }

        void number(function<void(int)> printNumber) 
        {
            for(int i = 1; i <= n; i++) {
                if(i % 3 == 0 || i % 5 == 0) continue;
                unique_lock<mutex> lock(mtx);
                cv.wait(lock, [this, i]() { return current == i; });
                printNumber(i);
                current++;
                cv.notify_all();
            }
        }
};

int main()
{
    // Example 1: FizzBuzz with n=15
    FizzBuzz fb1(15);
    thread t1(&FizzBuzz::fizz, &fb1, []() { cout << "fizz "; });
    thread t2(&FizzBuzz::buzz, &fb1, []() { cout << "buzz "; });
    thread t3(&FizzBuzz::fizzbuzz, &fb1, []() { cout << "fizzbuzz "; });
    thread t4(&FizzBuzz::number, &fb1, [](int x) { cout << x << " "; });
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    cout << "\n\n";

    // Example 2: FizzBuzz with n=10
    FizzBuzz fb2(10);
    thread t5(&FizzBuzz::fizz, &fb2, []() { cout << "fizz "; });
    thread t6(&FizzBuzz::buzz, &fb2, []() { cout << "buzz "; });
    thread t7(&FizzBuzz::fizzbuzz, &fb2, []() { cout << "fizzbuzz "; });
    thread t8(&FizzBuzz::number, &fb2, [](int x) { cout << x << " "; });
    
    t5.join();
    t6.join();
    t7.join();
    t8.join();
    cout << "\n";

    return 0;
}