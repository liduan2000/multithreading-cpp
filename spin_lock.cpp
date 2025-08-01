#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

class SpinLock {
  public:
    SpinLock() : flag_(ATOMIC_FLAG_INIT) {}

    void lock() {
        while (flag_.test_and_set(std::memory_order_acquire)) { std::this_thread::yield(); }
    }

    void unlock() { flag_.clear(std::memory_order_release); }

  private:
    std::atomic_flag flag_;
};

int main() {
    SpinLock spinLock;
    const int N = 4;
    int count = 0;
    std::vector<std::thread> threads;
    auto increment = [&]() {
        for (int i = 0; i < 1000; ++i) {
            spinLock.lock();
            ++count;
            spinLock.unlock();
        }
    };
    for (int i = 0; i < N; ++i) { threads.emplace_back(increment); }
    for (auto& t : threads) { t.join(); }

    std::cout << count << std::endl;

    return 0;
}
