// Copyright 2018 Event Vision Library.
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <deque>
#include <tuple>
#include "unistd.h"

#include <libcaercpp/libcaer.hpp>
#include <libcaercpp/devices/davis.hpp>

#include <evl/core/types.hpp>
#include <evl/core/shutdown.hpp>
#include <evl/core/davisio.hpp>

int main() {
    int lifetime = 2000;     // micro sec
    int buffersize = 50000;
    EventBuffer buffer(buffersize);

    std::thread t1(bufferData, &buffer);
    std::thread t2(loop_readData, &buffer, lifetime);

    t1.join();
    t2.join();
    return 0;
}
