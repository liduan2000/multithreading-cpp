#include <future>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

using ull = unsigned long long;
constexpr ull SIZE = 100000000;
constexpr int HW_CON_GUESS = 4;

void sumUp(std::promise<ull>&& promise, const std::vector<int>& values, ull begin, ull end) {
    ull tempSum = 0;
    for (ull i = begin; i < end; ++i) tempSum += values[i];
    promise.set_value(tempSum);
}

int main() {
    // get concurrency
    int hwCon = std::thread::hardware_concurrency();
    int hwConReal = hwCon == 0 ? HW_CON_GUESS : hwCon;

    std::cout << "Will execute on " << hwConReal << " threads" << std::endl;

    // generate random values
    std::vector<int> randValues;
    randValues.reserve(SIZE);

    std::mt19937 engine(std::random_device{}());
    std::uniform_int_distribution<> uniformDist(1, 10);
    for (long long i = 0; i < SIZE; ++i) randValues.push_back(uniformDist(engine));

    auto start = std::chrono::system_clock::now();
    // calculate ranges and start threads
    std::vector<std::promise<ull>> promises(hwConReal);
    std::vector<std::future<ull>> futures(hwConReal);
    std::vector<std::thread> threads(hwConReal);
    ull base = SIZE / hwConReal;
    ull reminder = SIZE % hwConReal;
    ull begin = 0;
    for (int i = 0; i < hwConReal; ++i) {
        ull end = begin + base;
        if (i < reminder) ++end;
        futures[i] = promises[i].get_future();
        threads[i] = std::thread(sumUp, std::move(promises[i]), std::ref(randValues), begin, end);
        begin = end;
    }

    ull sum = 0;
    for (auto& future : futures) sum += future.get();

    std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
    std::cout << "Time for addition: " << dur.count() << " seconds" << std::endl;
    std::cout << "Result: " << sum << std::endl;

    for (auto& thread : threads) thread.join();

    return 0;
}