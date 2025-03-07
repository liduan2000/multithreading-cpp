#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::mutex gLock;
static int sharedValue = 0;

void sharedValueIncrementLock() {
    gLock.lock();
    ++sharedValue;
    gLock.unlock();
}

void sharedValueIncrementLockGuard() {
    std::lock_guard<std::mutex> guard(gLock);
    ++sharedValue;
}

int main() {
    const int N = 100;
    std::vector<std::thread> threads;
    for (int i = 0; i < N; ++i) { threads.emplace_back(sharedValueIncrementLock); }

    for (int i = 0; i < N; ++i) { threads[i].join(); }

    std::cout << "Shared value: " << sharedValue << std::endl;
    return 0;
}