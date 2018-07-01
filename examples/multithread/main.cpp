// Copyright 2018 Event Vision Library.
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <tuple>

#include <boost/circular_buffer.hpp>
#include "unistd.h"
#include <evl/core/types.hpp>

std::mutex test_mtx;

void disp_(EventTuple x ) {
    std::cout << std::get<0>(x) << ' ' << \
    std::get<1>(x) << ' ' << std::get<2>(x) << std::endl;
}

// TUPLE VERSION
void loop_task3(EventBuffer *buffer) {
    // int i = 0;
    int32_t _ts = 0;
    uint16_t _x = 100;
    uint16_t _y = 120;
    bool _pol = true;
    while (1) {
        usleep(2);
        EventTuple tup = std::make_tuple(_ts, _x, _y, _pol);
        test_mtx.lock();
        (*buffer).push_front(tup);
        _ts += 1;
        test_mtx.unlock();
    }
}

std::vector<EventTuple> readLifetimeData(EventBuffer *buffer, int lifetime) {
    std::vector<EventTuple> v;
    test_mtx.lock();

    EventTuple tup = (*buffer).front();    // get first element
    int starttime = std::get<0>(tup);
    (*buffer).pop_front();                  // remove first element
    v.push_back(tup);
    for (int i = 0; i < (*buffer).size(); i++) {
        EventTuple tup = (*buffer).front();    // get first element
        if (std::get<0>(tup) > starttime - lifetime) {
            (*buffer).pop_front();                  // remove first element
            v.push_back(tup);
        } else {
          test_mtx.unlock();
          break;
        }
    }
    return v;
}

void loop_task4(EventBuffer *buffer, int lifetime) {
    for (int i = 0; i < 10000; i++) {
        usleep(100000);      // micro sec
        std::vector<EventTuple> v = readLifetimeData(buffer, lifetime);

        std::cout << "[Thread2] DATA READING =============" << std::endl;
        std::cout << "[Thread2] Lifetime >>> " << lifetime << std::endl;
        std::for_each((v).begin(), (v).end(), disp_);
        std::cout << "[Thread2] DATA READ DONE ===========" << std::endl;
    }
}


int main() {
    int lifetime = 100;
    EventBuffer buffer(50000);

    // std::thread t1(loop_task1, &ts, &x, &y, &pol);
    std::thread t1(loop_task3, &buffer);
    std::thread t2(loop_task4, &buffer, lifetime);

    t1.join();
    t2.join();
    return 0;
}
