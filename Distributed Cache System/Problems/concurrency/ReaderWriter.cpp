#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>

// Time Complexity: O(1) for all operations (acquire/release)
// Space Complexity: O(1) - constant space for counters and synchronization primitives
class ReaderWriterLock {
private:
    mutable std::mutex mtx;
    std::condition_variable cv;
    int readers = 0;
    int writers = 0;
    int waiting_writers = 0;

public:
    void acquire_read() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return waiting_writers == 0 && writers == 0; });
        readers++;
    }

    void release_read() {
        std::unique_lock<std::mutex> lock(mtx);
        readers--;
        if (readers == 0) cv.notify_all();
    }

    void acquire_write() {
        std::unique_lock<std::mutex> lock(mtx);
        waiting_writers++;
        cv.wait(lock, [this] { return readers == 0 && writers == 0; });
        waiting_writers--;
        writers++;
    }

    void release_write() {
        std::unique_lock<std::mutex> lock(mtx);
        writers--;
        cv.notify_all();
    }
};

// RAII wrappers
// Time Complexity: O(1) - constructor and destructor are O(1)
// Space Complexity: O(1) - stores reference to lock
class ReadLock {
private:
    ReaderWriterLock& lock;
public:
    ReadLock(ReaderWriterLock& l) : lock(l) { lock.acquire_read(); }
    ~ReadLock() { lock.release_read(); }
};

// Time Complexity: O(1) - constructor and destructor are O(1)
// Space Complexity: O(1) - stores reference to lock
class WriteLock {
private:
    ReaderWriterLock& lock;
public:
    WriteLock(ReaderWriterLock& l) : lock(l) { lock.acquire_write(); }
    ~WriteLock() { lock.release_write(); }
};

// Demo
int main() {
    ReaderWriterLock lock;
    int shared_data = 0;

    auto reader = [&](int id) {
        for (int i = 0; i < 3; i++) {
            ReadLock rl(lock);
            std::cout << "Reader " << id << " reads: " << shared_data << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    };

    auto writer = [&](int id) {
        for (int i = 0; i < 2; i++) {
            WriteLock wl(lock);
            shared_data++;
            std::cout << "Writer " << id << " writes: " << shared_data << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    };

    std::vector<std::thread> threads;
    threads.push_back(std::thread(reader, 1));
    threads.push_back(std::thread(reader, 2));
    threads.push_back(std::thread(writer, 1));
    threads.push_back(std::thread(reader, 3));
    threads.push_back(std::thread(writer, 2));

    for (auto& t : threads) t.join();

    return 0;
}
