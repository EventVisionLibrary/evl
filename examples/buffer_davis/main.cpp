// Copyright 2018 Event Vision Library.
#include <iostream>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <evl/core/types.hpp>
#include <evl/core/buffer_davis.hpp>
#include <evl/core/read_buffer.hpp>
#include <evl/utils/event_utils.hpp>

void loop_readData(EventBuffer *buffer, int lifetime) {
    usleep(100000);      // micro sec
    while (1) {
        usleep(100000);      // micro sec. calling frequency
        std::vector<EventTuple> v = readBufferOnLifetime(buffer, lifetime);

        std::cout << "[Thread2] DATA READING =============" << std::endl;
        std::cout << "[Thread2] Lifetime >>> " << lifetime << std::endl;
        std::for_each((v).begin(), (v).end(), disp_event);
        std::cout << "[Thread2] DATA READ DONE ===========" << std::endl;
    }
}

void loop_readImage(EventBuffer *buffer, int lifetime) {
    cv::namedWindow("image", 1);
    usleep(100000);      // micro sec
    while (1) {
        usleep(100000);      // micro sec. calling frequency
        std::vector<EventTuple> v = readBufferOnLifetime(buffer, lifetime);
        cv::Mat img = events_to_mat(v);
        cv::imshow("image", img);
        cv::waitKey(1);
    }
}

int main() {
    int lifetime = 2000;     // micro sec
    int buffersize = 50000;
    EventBuffer buffer(buffersize);
    std::thread t1(bufferData, &buffer);
    loop_readData(&buffer, lifetime);
    t1.join();
    return 0;
}
