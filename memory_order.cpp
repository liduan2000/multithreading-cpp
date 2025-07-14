#include <atomic>
#include <iostream>
#include <string>
#include <thread>

std::atomic<std::string*> ptr;
int data;
std::atomic<int> atomicData;

void produce() {
    std::string* p = new std::string("C++ 11");
    data = 100;
    atomicData.store(1, std::memory_order_relaxed);
    ptr.store(p, std::memory_order_release);
}

void consume() {
    std::string* p = nullptr;
    while (!(p = ptr.load(std::memory_order_acquire))) std::this_thread::yield();
    std::cout << "atomicData: " << atomicData.load(std::memory_order_relaxed) << std::endl;
    std::cout << "data: " << data << std::endl;
    std::cout << "p: " << *p << std::endl;
}

int main() {
    std::thread producer(produce);
    std::thread consumer(consume);
    producer.join();
    consumer.join();

    delete ptr;

    return 0;
}
