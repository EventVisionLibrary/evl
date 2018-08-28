// Copyright 2018 Event Vision Library.
#include <thread>
#include <evl/core/types.hpp>
#include <evl/core/store_buffer.hpp>
#include <evl/core/save_csv.hpp>

int main() {
    int lifetime = 1000;     // micro sec
    int buffersize = 50000;
    evl::EventBuffer buffer(buffersize);
    char original_file[] = "../../data/sample.csv";
    std::string new_file = "../../data/test.csv";

    std::thread t1(evl::storeBufferFromCsv, &buffer, original_file);
    std::thread t2(evl::saveBuffer, &buffer, lifetime, new_file);

    t1.join();
    t2.join();
    return 0;
}
