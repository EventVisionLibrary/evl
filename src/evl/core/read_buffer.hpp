// Copyright 2018 Event Vision Library.
#ifndef SRC_EVL_CORE_READ_BUFFER_HPP_
#define SRC_EVL_CORE_READ_BUFFER_HPP_

#include <vector>
#include "types.hpp"

std::vector<EventTuple> readBufferOnLifetime(EventBuffer *buffer, int lifetime);

std::vector<EventTuple> readBufferOnNumber(EventBuffer *buffer, int number);

#endif  // SRC_EVL_CORE_READ_BUFFER_HPP_
