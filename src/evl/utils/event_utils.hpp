// Copyright 2018 Event Vision Library.
#ifndef SRC_EVL_UTILS_EVENT_UTILS_HPP_
#define SRC_EVL_UTILS_EVENT_UTILS_HPP_

#include <vector>
#include <opencv2/core/core.hpp>

#include <evl/core/types.hpp>

namespace evl {
  void disp_event(EventTuple x);

  cv::Mat events_to_mat(std::vector<EventTuple> events);
}  // namespace evl

#endif    // SRC_EVL_UTILS_EVENT_UTILS_HPP_
