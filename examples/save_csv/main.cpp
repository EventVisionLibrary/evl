// Copyright 2018 Event Vision Library.
#include <thread>
#include <evl/core/types.hpp>
#include <evl/core/store_buffer.hpp>
#include <evl/core/save_csv.hpp>

int main() {
    int lifetime = 1000;     // micro sec
    int buffersize = 50000;
    evl::EventBuffer buffer(buffersize);
    char orgFname[] = "../../data/7.csv";
    std::string newFname = "test.txt";

    std::thread t1(evl::storeBufferCSV, &buffer, orgFname);
    std::thread t2(evl::saveBuffer, &buffer, lifetime, newFname);

    t1.join();
    t2.join();
    return 0;
}
