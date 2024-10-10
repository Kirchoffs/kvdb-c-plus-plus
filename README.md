# Notes
## Build & Test & Run
```
>> mkdir build
>> cd build
>> cmake ..

>> make
>> ./kvdb-c-plus-plus

>> ctest
>> ctest --verbose
```

## VS Code
### VS Code IntelliSense
Open command palette (cmd+shift+p) and select `C/C++: Edit Configurations (JSON)`. Add `includePath` to the `configuration` object. For example:
``` json
{
    "configurations": [
        {
            "name": "Mac",
            "includePath": [
                "${workspaceFolder}/src/include",
                "${workspaceFolder}/tests"
            ],
            "defines": [],
            "macFrameworkPath": [
                "/Library/Developer/CommandLineTools/SDKs/MacOSX14.sdk/System/Library/Frameworks"
            ],
            "compilerPath": "/opt/homebrew/opt/llvm/bin/clang++",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "macos-clang-arm64"
        }
    ],
    "version": 4
}
```

## C++ Macro
### Token Pasting Operator
`##` is the token pasting operator. It concatenates two tokens into a single token. For example:
``` C++
#include <iostream>

#define CONCAT(noun, preposition, pronoun) noun##_##preposition##_##pronoun

int main() {
    int answer_to_everything = 42;
    std::cout << CONCAT(answer, to, everything) << std::endl;
    return 0;
}
```

## C++ Details
### Virtual Destructor
#### Why do we need a virtual destructor?
When a derived class object is deleted through a pointer to a base class with a non-virtual destructor, the destructor of the base class is called but not the destructor of the derived class.

### Lock
#### Difference between `std::unique_lock`, `std::scoped_lock` and `std::lock_guard`
``` C++
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

// Shared resource and mutexes
int resource = 0;
std::mutex mtx1, mtx2;

// Example using std::lock_guard
void lock_guard_example() {
    std::lock_guard<std::mutex> lock(mtx1); // Locks mtx1 on construction
    resource += 1;
    std::cout << "Lock guard: resource = " << resource << "\n";

    // Mutex automatically unlocked when lock goes out of scope
}

// Example using std::unique_lock
void unique_lock_example_1() {
    std::unique_lock<std::mutex> lock(mtx1); // Locks mtx1
    resource += 2;
    std::cout << "Unique lock 1: resource = " << resource << "\n";

    lock.unlock(); // Explicitly unlock the mutex

    // Mutex automatically unlocked again (if still locked) when lock goes out of scope
}

// Example using std::unique_lock
void unique_lock_example_2() {
    std::unique_lock<std::mutex> lock1(mtx1, std::defer_lock); // Defer locking
    std::unique_lock<std::mutex> lock2(mtx2, std::defer_lock); // Defer locking

    // Manually lock the mutexes to avoid deadlocks
    std::lock(lock1, lock2); // Locks both mutexes safely

    // Critical section
    resource += 3;
    std::cout << "Unique lock 2: resource = " << resource << "\n";

    // Mutexes automatically unlocked when lock1 and lock2 go out of scope
}

// Example using std::scoped_lock
void scoped_lock_example() {
    std::scoped_lock lock(mtx1, mtx2); // Locks both mtx1 and mtx2
    resource += 4;
    std::cout << "Scoped lock: resource = " << resource << "\n";

    // Both mutexes automatically unlocked when lock goes out of scope
}

int main() {
    std::thread t1(lock_guard_example);
    std::thread t2(unique_lock_example_1);
    std::thread t3(unique_lock_example_2);
    std::thread t4(scoped_lock_example);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Main thread: resource = " << resource << "\n";

    return 0;
}
```

#### Read-Write Lock
After C++ 17, we can use `std::shared_mutex` along with `std::unique_lock` and `std::shared_lock` to implement a read-write lock.  
Before C++ 17, we can use `std::mutex`, `std::condition_variable` and `std::unique_lock` to implement a read-write lock.

``` C++
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

class ReadWriteLock {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int reader_count = 0;       // Number of active readers
    bool writer_active = false; // Is a writer active?

public:
    void lock_read() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() { return !writer_active; }); // Wait for no active writer
        ++reader_count;
    }

    void unlock_read() {
        std::unique_lock<std::mutex> lock(mtx);
        --reader_count;
        if (reader_count == 0) {
            cv.notify_all();
        }
    }

    void lock_write() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() { return !writer_active && reader_count == 0; }); // Wait for no readers or writers
        writer_active = true;
    }

    void unlock_write() {
        std::unique_lock<std::mutex> lock(mtx);
        writer_active = false;
        cv.notify_all();
    }
};

int shared_resource = 0;
ReadWriteLock rw_lock;

void reader(int id) {
    rw_lock.lock_read();
    std::cout << "Reader " << id << " is reading: " << shared_resource << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    rw_lock.unlock_read();
}

void writer(int id) {
    rw_lock.lock_write();
    ++shared_resource;
    std::cout << "Writer " << id << " updated resource to: " << shared_resource << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    rw_lock.unlock_write();
}

int main() {
    std::vector<std::thread> threads;

    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(writer, i);
        threads.emplace_back(reader, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
```
