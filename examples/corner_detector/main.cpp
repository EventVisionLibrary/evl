
/*
Copyright 2018 Event Vision Library.

Implementation of "Fast Event-based Harris Corner Detection Exploiting the Advantages of Event-driven Cameras",
 https://www.semanticscholar.org/paper/Fast-event-based-Harris-corner-detection-exploiting-Vasco-Glover/2d57a3b949a82e2ff9d6dc37d0a53e8c1514af22
*/

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <evl/core/types.hpp>
#include <evl/core/store_buffer.hpp>
#include <evl/core/read_buffer.hpp>
#include <evl/utils/event_utils.hpp>

#include <iostream>
#include <queue>
#include <deque>
#include <utility>
#include <string>
#include <vector>

#define W 240
#define H 180
#define L 7
#define NUM_EVENTS 2000
#define POSITIVE true
#define NEGATIVE false
#define HARRIS_THRESHOLD 20

std::vector<cv::Point2f> EventHarrisDetector(
        std::queue<evl::EventTuple>* ev_slice) {
    cv::Mat binaryOn = cv::Mat::zeros(H, W, CV_8UC1);
    cv::Mat binaryOff = cv::Mat::zeros(H, W, CV_8UC1);

    cv::Mat h = cv::getGaussianKernel(L, 1, CV_32F);
    h = h * h.t();

    std::vector <cv::Point2f> corners;
    std::deque<std::pair<int, int>>
            pos_queue(NUM_EVENTS+1), neg_queue(NUM_EVENTS+1);
    while (!ev_slice->empty()) {
        evl::EventTuple event_cur = ev_slice->front();
        ev_slice->pop();
        double x = std::get<1>(event_cur);
        double y = std::get<2>(event_cur);
        double pol = std::get<3>(event_cur);

        if (x <= L || x >= W - L || y <= L || y >= H - L)
            continue;

        if (pol == POSITIVE) {
            binaryOn.at<uchar>(y, x) = 1;
            pos_queue.push_back(std::make_pair(x, y));
            if (pos_queue.size() == (NUM_EVENTS + 1)) {
                std::pair<int, int> oldestPos = pos_queue.front();
                binaryOn.at<uchar>(oldestPos.second, oldestPos.first) = 0;
                pos_queue.pop_front();
            }

            cv::Rect ROI = cv::Rect(x - L, y - L, 2 * L + 1, 2 * L + 1);
            cv::Mat mat_roi = binaryOn(ROI);
            cv::Mat dx, dy;
            cv::Sobel(mat_roi, dx, CV_64F, 1, 0, 7, 0.01, cv::BORDER_CONSTANT);
            cv::Sobel(mat_roi, dy, CV_64F, 0, 1, 7, 0.01, cv::BORDER_CONSTANT);
            cv::Mat dx2 = dx.mul(dx);
            cv::Mat dy2 = dy.mul(dy);
            cv::Mat dxy = dx.mul(dy);
            cv::Mat h_ = cv::getGaussianKernel(2 * L + 1, 1);
            cv::Mat h = h_ * h_.t();

            cv::Mat dx2_ = dx2.mul(h);
            cv::Mat dy2_ = dy2.mul(h);
            cv::Mat dxy_ = dxy.mul(h);
            double a = cv::sum(dx2_)[0];
            double d = cv::sum(dy2_)[0];
            double b = cv::sum(dxy_)[0];
            double c = b;
            Eigen::Matrix2d M;
            M << a, b,
                    c, d;
            double score = M.determinant() - 0.04 * (M.trace() * M.trace());

            if (score > HARRIS_THRESHOLD)
                corners.push_back(cv::Point2f(x, y));
        } else {
            binaryOff.at<uchar>(y, x) = 1;
            neg_queue.push_back(std::make_pair(x , y));
            if (neg_queue.size() == (NUM_EVENTS+1)) {
                std::pair<int, int> oldestPos = neg_queue.front();
                binaryOff.at<uchar>(oldestPos.second, oldestPos.first) = 0;
                neg_queue.pop_front();
            }

            cv::Rect ROI = cv::Rect(x - L, y - L, 2 * L + 1, 2 * L + 1);
            cv::Mat mat_roi = binaryOff(ROI);
            cv::Mat dx, dy;
            cv::Sobel(mat_roi, dx, CV_64F, 1, 0, 7, 0.01, cv::BORDER_CONSTANT);
            cv::Sobel(mat_roi, dy, CV_64F, 0, 1, 7, 0.01, cv::BORDER_CONSTANT);
            cv::Mat dx2 = dx.mul(dx);
            cv::Mat dy2 = dy.mul(dy);
            cv::Mat dxy = dx.mul(dy);
            cv::Mat h_ = cv::getGaussianKernel(2 * L + 1, 1);
            cv::Mat h = h_ * h_.t();

            cv::Mat dx2_ = dx2.mul(h);
            cv::Mat dy2_ = dy2.mul(h);
            cv::Mat dxy_ = dxy.mul(h);
            double a = cv::sum(dx2_)[0];
            double d = cv::sum(dy2_)[0];
            double b = cv::sum(dxy_)[0];
            double c = b;
            Eigen::Matrix2d M;
            M << a, b,
                    c, d;
            double score = M.determinant() - 0.04 * (M.trace() * M.trace());

            if (score > HARRIS_THRESHOLD)
                corners.push_back(cv::Point2f(x, y));
        }
    }

    return corners;
}

void CornerDetection(const std::string& fname, const double& lifetime_sec) {
    // Reading all event data from csv.
    FILE *fp;
    fp = fopen(fname.c_str(), "r");
    if (fp == NULL) {
        printf("%sThe file cannot be opened!\n", fname.c_str());
        return;
    }

    int ret;
    double ts, x, y, pol_raw;
    std::queue<evl::EventTuple> que;
    while ((ret=fscanf(fp, "%lf %lf %lf %lf", &ts, &x, &y, &pol_raw)) != EOF) {
        bool pol = static_cast<bool>(pol_raw);
        evl::EventTuple tup = std::make_tuple(ts, x, y, pol);
        que.push(tup);
    }
    fclose(fp);

    std::vector<evl::EventTuple> v;
    std::queue<evl::EventTuple> time_slice;
    while (!que.empty()) {
        // Create image from events.
        double start_time = std::get<0>(que.front());
        double current_time = start_time;
        while (!que.empty() && current_time < start_time + lifetime_sec) {
            v.push_back(que.front());
            time_slice.push(que.front());
            current_time = std::get<0>(que.front());
            que.pop();
        }
        cv::Mat img = evl::convertEventsToMat(v, true);

        // Harris corner detection
        cv::Mat harris_img = img.clone();
        std::vector <cv::Point2f> corners = EventHarrisDetector(&time_slice);
        std::vector<cv::Point2f>::const_iterator it_corner = corners.begin();
        for (; it_corner != corners.end(); ++it_corner) {
            cv::circle(harris_img, cv::Point(it_corner->x, it_corner->y),
                   2, cv::Scalar(0, 255, 0), -1);
        }

        // free memory
        std::vector<evl::EventTuple>().swap(v);

        // Visualization
        cv::imshow("DVS", img);
        cv::imshow("Harris", harris_img);
        // Note: This "3" is a magic number for smooth execution.
        cv::waitKey(3);
    }

    return;
}


int main(int argc, char* argv[]) {
    if (argc <=1) {
        printf("Usage: ./corner_detector_bin ../../data/sample.txt\n");
        printf("Note: This detector assumes uzh-rpg event data format. "
                       "Image size is 240*180 "
                       "and Timestamp must be given in seconds.\n");
        return 0;
    }

    double lifetime_sec = 0.01;
    CornerDetection(std::string(argv[1]), lifetime_sec);

    return 0;
}
