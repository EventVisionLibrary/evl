// Copyright 2018 Event Vision Library.
#ifndef SRC_EVL_CORE_DAVISIO_HPP_
#define SRC_EVL_CORE_DAVISIO_HPP_

#include <mutex>
#include <vector>
#include <libcaercpp/libcaer.hpp>
#include <libcaercpp/devices/davis.hpp>

#include "types.hpp"

extern std::mutex mtx;

void disp(EventTuple x);

Davis initializeDavis(void);
void bufferData(EventBuffer *buffer);

std::vector<EventTuple> readBufferOnLifetime(EventBuffer *buffer, int lifetime);

std::vector<EventTuple> readBufferOnNumber(EventBuffer *buffer, int number);

void loop_readData(EventBuffer *buffer, int lifetime);

#endif  // SRC_EVL_CORE_DAVISIO_HPP_
