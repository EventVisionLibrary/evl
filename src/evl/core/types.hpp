// Copyright 2018 Event Vision Library.
#ifndef SRC_EVL_CORE_TYPES_HPP_
#define SRC_EVL_CORE_TYPES_HPP_

#include <tuple>
#include <boost/circular_buffer.hpp>

typedef std::tuple<int32_t, uint16_t, uint16_t, bool> EventTuple;
typedef boost::circular_buffer<EventTuple> EventBuffer;

#endif   // SRC_EVL_CORE_TYPES_HPP_
