// Copyright 2018 Event Vision Library.
#include <iostream>
#include <thread>
#include <vector>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <evl/core/read_buffer.hpp>
#include <evl/core/store_buffer.hpp>
#include <evl/core/types.hpp>
#include <evl/utils/event_utils.hpp>

void streamEvent(evl::EventBuffer *buffer, int lifetime) {
    usleep(10000);      // micro sec
    while (1) {
        usleep(100000);      // micro sec. calling frequency
        // std::vector<evl::EventTuple> v = evl::readBufferOnLifetime(buffer, lifetime);
        std::vector<evl::EventTuple> v = evl::readBufferOnNumber(buffer, 100);
        std::cout << "[Thread2] DATA READING =============" << std::endl;
        std::cout << "[Thread2] Lifetime >>> " << lifetime << std::endl;
        std::for_each((v).begin(), (v).end(), evl::printEvent);
        std::cout << "[Thread2] DATA READ DONE ===========" << std::endl;
    }
}

void streamEventAsImage(evl::EventBuffer *buffer, int lifetime) {
    cv::namedWindow("image", 1);
    usleep(10000);      // micro sec
    while (1) {
        usleep(100000);      // micro sec. calling frequency
        std::vector<evl::EventTuple> v = evl::readBufferOnLifetime(buffer, lifetime);
        // std::vector<evl::EventTuple> v = evl::readBufferOnNumber(buffer, 100);
        cv::Mat img = evl::convertEventsToMat(v, true);
        cv::imshow("image", img);
        cv::waitKey(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Error! specify event source file or davis." << std::endl;
        std::exit(1);
    }
    std::string event_source = argv[1];
    int lifetime = 5000;    // micro sec
    int buffersize = 50000;

    evl::EventBuffer buffer(buffersize);

    if (event_source == "davis") {
        std::cout << "read from Davis." << std::endl;
        std::thread t1(evl::storeBufferFromDavis, &buffer);
        streamEventAsImage(&buffer, lifetime);
        t1.join();
    } else if (event_source.find("csv") != std::string::npos) {
        std::cout << "read from Csv." << std::endl;
        std::thread t1(evl::storeBufferFromCsv, &buffer, argv[1]);
        streamEventAsImage(&buffer, lifetime);
        // streamEvent(&buffer, lifetime);
        t1.join();
    } else {
        std::cout << "Error! wrong event source name." << std::endl;
        std::exit(1);
    }
    return 0;
}
