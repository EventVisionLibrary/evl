// Copyright 2018 Event Vision Library.
#ifndef SRC_EVL_UTILS_EVENT_UTILS_HPP_
#define SRC_EVL_UTILS_EVENT_UTILS_HPP_

#include <vector>
#include <opencv2/core/core.hpp>

#include <evl/core/types.hpp>

namespace evl {
void printEvent(EventTuple x);
cv::Mat convertEventsToMat(std::vector<EventTuple> events, bool with_pol=false);
}  // namespace evl

#endif    // SRC_EVL_UTILS_EVENT_UTILS_HPP_
