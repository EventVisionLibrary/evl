// Copyright 2018 Event Vision Library.
#include "read_buffer.hpp"

#include <unistd.h>     //  for sleep function
#include <mutex>
#include <vector>

#include "common.hpp"
#include "shutdown.hpp"
#include "types.hpp"

std::mutex mtx;


namespace evl {
std::vector<EventTuple> readBufferOnLifetime(EventBuffer *buffer,
    int lifetime) {
    std::vector<EventTuple> v;

    mtx.lock();
    EventTuple tup = (*buffer).front();    // get first element
    int32_t starttime = std::get<0>(tup);
    v.push_back(tup);

    for (size_t cnt = 1; cnt < (*buffer).capacity(); cnt++) {
        EventTuple tup = (*buffer)[cnt];    // get first element
        if (std::get<0>(tup) == 0) {
        break;
        } else if (std::get<0>(tup) > starttime - lifetime) {
            v.push_back(tup);
        } else { break; }
    }
    mtx.unlock();
    return v;
}

std::vector<EventTuple> readBufferOnNumber(EventBuffer *buffer, int number) {
    std::vector<EventTuple> v;

    mtx.lock();
    EventTuple tup = (*buffer).front();    // get first element
    v.push_back(tup);
    int size = v.size();
    for (size_t cnt = 1; cnt < (*buffer).capacity(); cnt++) {
        EventTuple tup = (*buffer)[cnt];    // get first element
        if (std::get<0>(tup) == 0) {
        break;
        } else if (size < number) {
            v.push_back(tup);
        } else { break; }
    }
    mtx.unlock();
    return v;
}
}  // namespace evl
