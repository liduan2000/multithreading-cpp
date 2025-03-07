#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

class A {
  public:
    int read() const {
        std::shared_lock<std::shared_mutex> l(sm_);
        return n_;
    }

    int write() {
        std::unique_lock<std::shared_mutex> l(sm_);
        return ++n_;
    }

    int getN() const { return n_; }

  private:
    mutable std::shared_mutex sm_;
    int n_ = 0;
};

int main() {
    const int N = 100;
    A a;
    std::vector<std::thread> readThreads;
    std::vector<std::thread> writeThreads;
    for (int i = 0; i < N; ++i) {
        readThreads.emplace_back([&a]() { a.read(); });
        writeThreads.emplace_back([&a]() { a.write(); });
    }
    for (int i = 0; i < N; ++i) {
        readThreads[i].join();
        writeThreads[i].join();
    }

    std::cout << a.getN() << std::endl;
    return 0;
}