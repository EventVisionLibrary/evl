// Copyright 2018 Event Vision Library.
#include <thread>
#include <evl/core/types.hpp>
#include <evl/core/buffer_csv.hpp>
#include <evl/core/save_csv.hpp>

int main() {
    int lifetime = 1000;     // micro sec
    int buffersize = 50000;
    evl::EventBuffer buffer(buffersize);
    char fname[] = "../../data/7.csv";
    std::string filename = "test.txt";

    std::thread t1(evl::bufferData, &buffer, fname);
    std::thread t2(evl::loop_saveData, &buffer, lifetime, filename);

    t1.join();
    t2.join();
    return 0;
}
