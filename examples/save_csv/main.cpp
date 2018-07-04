// Copyright 2018 Event Vision Library.
#include <thread>
#include <evl/core/types.hpp>
#include <evl/core/buffer_csv.hpp>
#include <evl/core/save_csv.hpp>

int main() {
    int lifetime = 1000;     // micro sec
    int buffersize = 50000;
    EventBuffer buffer(buffersize);
    char fname[] = "../../data/7.csv";

    std::thread t1(bufferData, &buffer, fname);
    std::thread t2(loop_saveData, &buffer, lifetime);

    t1.join();
    t2.join();
    return 0;
}
