#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <numeric>
#include <random>
#include <vector>

constexpr long long SIZE = 100000000;

int main() {
    std::vector<int> randValues;
    randValues.reserve(SIZE);

    // random values
    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_int_distribution<> uniformDist(1, 10);
    for (long long i = 0; i < SIZE; ++i) randValues.push_back(uniformDist(engine));

    auto start = std::chrono::system_clock::now();

    unsigned long long sum = std::accumulate(randValues.begin(), randValues.end(), 0);

    std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
    std::cout << "Time for addition " << dur.count() << " seconds" << std::endl;
    std::cout << "Result: " << sum << std::endl;

    return 0;
}