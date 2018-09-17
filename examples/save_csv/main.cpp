// Copyright 2018 Event Vision Library.
#include <iostream>
#include <string>
#include <thread>
#include <evl/core/types.hpp>
#include <evl/core/store_buffer.hpp>
#include <evl/core/save_csv.hpp>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Error! specify event source." << std::endl;
        std::exit(1);
    }
    std::string event_source = argv[1];
    int lifetime = 1000;     // micro sec
    int buffersize = 50000;
    evl::EventBuffer buffer;
    std::string new_file = "../../data/test.csv";

    if (event_source == "Davis") {
        std::cout << "read from Davis." << std::endl;
        std::thread t1(evl::storeBufferFromDavis, &buffer);
        std::thread t2(evl::saveBuffer, &buffer, lifetime, new_file);
        t1.join();
        t2.join();
    } else if (event_source.find("csv") != std::string::npos) {
        std::cout << "read from Csv." << std::endl;
        std::thread t1(evl::storeBufferFromCsv, &buffer, argv[1]);
        std::thread t2(evl::saveBuffer, &buffer, lifetime, new_file);
        t1.join();
        t2.join();
    } else {
        std::cout << "Error! wrong event source name." << std::endl;
        std::exit(1);
    }
    return 0;
}
