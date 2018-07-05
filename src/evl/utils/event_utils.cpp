// Copyright 2018 Event Vision Library.
#include <iostream>
#include <opencv2/core/core.hpp>
#include <evl/core/types.hpp>

#include "event_utils.hpp"

void disp_event(EventTuple x) {
    std::cout << ' ' << std::get<0>(x) << ' ' << \
    std::get<1>(x) << ' ' << std::get<2>(x) << ' ' << \
    std::get<3>(x) << std::endl;
}

cv::Mat events_to_mat(std::vector<EventTuple> events) {
    int W = 240;
    int H = 180;
    cv::Mat src = cv::Mat::zeros(H, W, CV_8UC1);
    for (auto iter = events.begin(); iter != events.end(); ++iter) {
        int x = std::get<1>(*iter);
        int y = std::get<2>(*iter);
        src.at<unsigned char>(y, x) = 255;  // assume binary image
    }
    return src;
}