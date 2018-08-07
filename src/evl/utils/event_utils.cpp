// Copyright 2018 Event Vision Library.
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <evl/core/types.hpp>

#include "event_utils.hpp"

namespace evl {
  void disp_event(EventTuple x) {
      std::cout << ' ' << std::get<0>(x) << ' ' << \
      std::get<1>(x) << ' ' << std::get<2>(x) << ' ' << \
      std::get<3>(x) << std::endl;
  }

  cv::Mat events_to_mat(std::vector<EventTuple> events, bool with_pol) {
      int W = 240;
      int H = 180;
      cv::Mat src;
      if (with_pol) {
        src = cv::Mat::zeros(H, W, CV_8UC3);
      } else {
        src = cv::Mat::zeros(H, W, CV_8UC1);
      }
      for (auto iter = events.begin(); iter != events.end(); ++iter) {
          int x = std::get<1>(*iter);
          int y = std::get<2>(*iter);
          if (with_pol) {
              bool pol = std::get<3>(*iter);
              cv::Vec3b *p = src.ptr<cv::Vec3b>(y, x);
              if (pol) {
                  *p = cv::Vec3b(0, 0, 255); // +1 -> red
              } else {
                  *p = cv::Vec3b(255, 0, 0); // -1 -> blue
              }
          } else {
              unsigned char *p = src.ptr<unsigned char>(y, x);
              *p = 255; 
          }
      }
      return src;
  }
}  // namespace evl
