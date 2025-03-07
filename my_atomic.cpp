#include <atomic>
#include <iostream>

class MyAtomic {
  private:
    int i_;
    double d_;
    std::string* sptr_;
};

int main() {
    std::cout << std::is_trivially_copyable<MyAtomic>::value << std::endl;
    std::cout << std::is_copy_constructible<MyAtomic>::value << std::endl;
    std::cout << std::is_move_constructible<MyAtomic>::value << std::endl;
    std::cout << std::is_copy_assignable<MyAtomic>::value << std::endl;
    std::cout << std::is_move_assignable<MyAtomic>::value << std::endl;
    std::cout << std::is_same<MyAtomic, typename std::remove_cv<MyAtomic>::type>::value << std::endl;

    std::atomic<MyAtomic> myAtomic;

    return 0;
}