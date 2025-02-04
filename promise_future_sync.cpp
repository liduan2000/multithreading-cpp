#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int sharedData = 0;

void doTask() { std::cout << "Data: " << sharedData << std::endl; }

void produce(std::promise<void>&& promise) {
    std::cout << "Producing..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    sharedData = 1;
    promise.set_value();
}

void consume(std::future<void>&& future) {
    std::cout << "Waiting..." << std::endl;
    future.wait();
    doTask();
    std::cout << "All done." << std::endl;
}

int main() {
    std::promise<void> promise;
    auto future = promise.get_future();

    std::thread producer(produce, std::move(promise));
    std::thread consumer(consume, std::move(future));

    producer.join();
    consumer.join();

    return 0;
}