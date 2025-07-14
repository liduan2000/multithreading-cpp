#include <atomic>
#include <iostream>

template <typename T>
T fetch_mult(std::atomic<T>& shared, T mult) {
    T oldValue = shared.load();
    while (!shared.compare_exchange_strong(oldValue, oldValue * mult));
    return oldValue;
}

int main() {
    std::atomic<int> data(5);
    std::cout << data << std::endl;
    fetch_mult(data, 5);
    std::cout << data << std::endl;

    return 0;
}
