#include <future>
#include <iostream>
#include <thread>

void product(std::promise<int>&& promise, int a, int b) { promise.set_value(a * b); }

struct Divide {
    void operator()(std::promise<int>&& promise, int a, int b) const { promise.set_value(a / b); }
};

int main() {
    // define the promises
    std::promise<int> prodPromise;
    std::promise<int> divPromise;

    // get the futures
    std::future<int> prodFuture = prodPromise.get_future();
    std::future<int> divFuture = divPromise.get_future();

    int a = 20, b = 10;
    // calculate concurrently
    std::thread prodThread(product, std::move(prodPromise), a, b);
    Divide divide;
    std::thread divThread(divide, std::move(divPromise), a, b);

    // get the results
    std::cout << a << "*" << b << "=" << prodFuture.get() << std::endl;
    std::cout << a << "/" << b << "=" << divFuture.get() << std::endl;

    prodThread.join();
    divThread.join();

    return 0;
}