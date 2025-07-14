#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

std::mutex coutMutex;

struct Divide {
    void operator()(std::promise<int>&& promise, int a, int b) const {
        try {
            if (b == 0) throw std::runtime_error("Illegal division by zero!");
            promise.set_value(a / b);
        } catch (...) { promise.set_exception(std::current_exception()); }
    }
};

struct Requestor {
    void operator()(std::shared_future<int> shdFuture) {
        std::lock_guard<std::mutex> l(coutMutex);
        std::cout << "thread id(" << std::this_thread::get_id() << "): ";
        try {
            std::cout << shdFuture.get() << std::endl;
        } catch (const std::exception& e) { std::cout << e.what() << std::endl; }
    }
};

int main() {
    // define the promise
    std::promise<int> divPromise;

    // get the shared future
    std::shared_future<int> shdFuture = divPromise.get_future();

    int a = 20, b = 10;
    // calculate in a separate thread
    Divide divide;
    std::thread divThread(divide, std::move(divPromise), a, b);

    Requestor requestor;
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) threads.emplace_back(requestor, shdFuture);

    divThread.join();
    for (auto& t : threads) t.join();

    return 0;
}
