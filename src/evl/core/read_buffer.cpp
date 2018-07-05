// Copyright 2018 Event Vision Library.
#include <iostream>
#include <vector>
#include <mutex>
#include "unistd.h"     //  for sleep function

#include "types.hpp"
#include "shutdown.hpp"
#include "common.hpp"

#include "read_buffer.hpp"

std::mutex mtx;

void disp_event(EventTuple x ) {
    std::cout << ' ' << std::get<0>(x) << ' ' << \
    std::get<1>(x) << ' ' << std::get<2>(x) << ' ' << \
    std::get<3>(x) << std::endl;
}

std::vector<EventTuple> readBufferOnLifetime(EventBuffer *buffer,
  int lifetime) {
    std::vector<EventTuple> v;

    mtx.lock();
    EventTuple tup = (*buffer).front();    // get first element
    int32_t starttime = std::get<0>(tup);
    (*buffer).pop_front();                  // remove first element
    v.push_back(tup);

    for (int cnt = 0; cnt < (*buffer).capacity(); cnt++) {
        EventTuple tup = (*buffer).front();    // get first element
        if (std::get<0>(tup) == 0) {
          break;
        } else if (std::get<0>(tup) > starttime - lifetime) {
            (*buffer).pop_front();                  // remove first element
            v.push_back(tup);
        } else {break;}
    }
    mtx.unlock();
    return v;
}

std::vector<EventTuple> readBufferOnNumber(EventBuffer *buffer, int number) {
    std::vector<EventTuple> v;

    mtx.lock();
    EventTuple tup = (*buffer).front();    // get first element
    (*buffer).pop_front();                  // remove first element
    v.push_back(tup);

    for (int cnt = 0; cnt < (*buffer).capacity(); cnt++) {
        EventTuple tup = (*buffer).front();    // get first element
        if (std::get<0>(tup) == 0) {
          break;
        } else if (v.size() < number) {
            (*buffer).pop_front();                  // remove first element
            v.push_back(tup);
        } else {
          break;
        }
    }
    mtx.unlock();
    return v;
}

// example
void loop_readData(EventBuffer *buffer, int lifetime) {
    usleep(100000);      // micro sec
    while (1) {
        usleep(100000);      // micro sec. calling frequency
        std::vector<EventTuple> v = readBufferOnLifetime(buffer, lifetime);

        std::cout << "[Thread2] DATA READING =============" << std::endl;
        std::cout << "[Thread2] Lifetime >>> " << lifetime << std::endl;
        std::for_each((v).rbegin(), (v).rend(), disp_event);
        std::cout << "[Thread2] DATA READ DONE ===========" << std::endl;
    }
}
