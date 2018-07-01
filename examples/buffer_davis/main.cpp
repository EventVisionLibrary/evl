// Copyright 2018 Event Vision Library.
#include <thread>
#include <evl/core/types.hpp>
#include <evl/core/buffer_davis.hpp>
#include <evl/core/read_buffer.hpp>

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
