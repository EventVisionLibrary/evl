// Copyright 2018 Event Vision Library.
#include "read_buffer.hpp"

#include "unistd.h"     //  for sleep function
#include <mutex>
#include <vector>

#include "common.hpp"
#include "shutdown.hpp"
#include "types.hpp"

std::mutex mtx;

#include <iostream>
namespace evl {
std::vector<EventTuple> readBufferOnLifetime(EventBuffer *buffer,
    int lifetime) {
    std::vector<EventTuple> v;

    mtx.lock();
    EventTuple tup = (*buffer).front();    // refer to first element
    int32_t start_time = std::get<0>(tup);

    while (1) {
        EventTuple tup = (*buffer).front();    // refer to first element
        std::cout << "arakawa" << std::endl;  
        if (std::get<0>(tup) == 0) {
            std::cout << "hoge" << std::endl; 
            (*buffer).pop();
            break;
        } else if (std::get<0>(tup) < start_time + lifetime) {
            (*buffer).pop();                  // remove first element
            v.push_back(tup);
            std::cout << "riku" << std::endl; 
        } else { 
            std::cout << "fuga" << std::endl;   // if next data is over lifetime, keep it in buffer. 
            break; 
        }
    }
    mtx.unlock();
    return v;
}

std::vector<EventTuple> readBufferOnNumber(EventBuffer *buffer, int number) {
    std::vector<EventTuple> v;

    mtx.lock();

    while (v.size() < number) {
        EventTuple tup = (*buffer).front();    // refer to first element
        (*buffer).pop();    // refer to first element
        if (std::get<0>(tup) != 0) {
            v.push_back(tup);
        } else {
            break;
        }
    }
    mtx.unlock();
    return v;
}
}  // namespace evl
