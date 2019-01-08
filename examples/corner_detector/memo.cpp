
/*
Copyright 2018 Event Vision Library.

Implementation of "Fast Event-based Harris Corner Detection Exploiting the Advantages of Event-driven Cameras",
 https://www.semanticscholar.org/paper/Fast-event-based-Harris-corner-detection-exploiting-Vasco-Glover/2d57a3b949a82e2ff9d6dc37d0a53e8c1514af22
*/
#include <iostream>
#include <thread>
#include <queue>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

#include <evl/core/types.hpp>
#include <evl/core/buffer_csv.hpp>
#include <evl/core/read_buffer.hpp>
#include <evl/utils/event_utils.hpp>



#define W 240
#define H 180
#define L 7
#define NUM_EVENTS 2000
#define POSITIVE true
#define NEGATIVE false



void CornerDetection(char* fname, int lifetime){
    //Reading all event data from csv and save it to queue.
    std::queue<evl::EventTuple> ev_queue;
    std::deque<std::pair<int, int>> pos_queue(NUM_EVENTS+1), neg_queue(NUM_EVENTS+1);

    FILE *fp;
    cv::Mat img;
    cv::Mat vSurfOn, vSurfOff, binaryOn, binaryOff;
    cv::Mat dx, dy, dx2, dy2, dxy, dx2_, dy2_, dxy_;
    cv::Mat h;
    double a, b, c, d;
    std::vector <cv::Point2f> corners;
    img = cv::Mat::zeros(H, W, CV_8UC3);
    vSurfOn = cv::Mat::zeros(H, W, CV_64FC1);
    vSurfOff = cv::Mat::zeros(H, W, CV_64FC1);
    binaryOn = cv::Mat::zeros(H, W, CV_8UC1);
    binaryOff = cv::Mat::zeros(H, W, CV_8UC1);

    h = cv::getGaussianKernel(L, 1, CV_32F);
    h = h * h.t();

    fp = fopen(fname, "r");
    if (fp == NULL) {
        printf("%sThe file cannot be opened!\n", fname);
        return;
    }
    int ret;

    double ts; double x; double y;
    double pol_raw; bool pol;

    while ((ret=fscanf(fp, "%lf,%lf,%lf,%lf", &ts, &x, &y, &pol_raw)) != EOF) {
        pol = static_cast<bool>(pol_raw);
        evl::EventTuple tup = std::make_tuple(ts, x, y, pol);
        ev_queue.push(tup);
    }
    fclose(fp);

    while(!ev_queue.empty()) {
        //Create image from events.
        evl::EventTuple event_cur = ev_queue.front();
        ts = std::get<0>(event_cur);
        x = std::get<1>(event_cur);
        y = std::get<2>(event_cur);
        pol = std::get<3>(event_cur);
        ev_queue.pop();

        if (x <= L or x >= W - L or y <= L or y >= H - L)
            continue;

        if (pol == POSITIVE) {
            vSurfOn.at<double>(y, x) = ts;
            img.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
            binaryOn.at<uchar>(y, x) = 1;
            pos_queue.push_back(std::make_pair(x,y));
            if (pos_queue.size() == (NUM_EVENTS+1)) {
                std::pair<int, int> oldestPos = pos_queue.front();
                pos_queue.pop_front();
                img.at<cv::Vec3b>(oldestPos.second, oldestPos.first) = cv::Vec3b(0, 0, 0);
                vSurfOn.at<double>(oldestPos.second, oldestPos.first) = 0;
                binaryOn.at<uchar>(oldestPos.second, oldestPos.first) = 0;
            }

            cv::Rect ROI = cv::Rect(x - L, y - L, 2 * L + 1, 2 * L + 1);
            cv::Mat mat_roi = binaryOn(ROI);
            cv::Sobel(mat_roi, dx, CV_64F, 1, 0, 7, 0.01, cv::BORDER_CONSTANT);
            cv::Sobel(mat_roi, dy, CV_64F, 0, 1, 7, 0.01, cv::BORDER_CONSTANT);
            dx2 = dx.mul(dx);
            dy2 = dy.mul(dy);
            dxy = dx.mul(dy);
            cv::Mat h_ = cv::getGaussianKernel(2 * L + 1, 1);
            cv::Mat h = h_ * h_.t();

            dx2_ = dx2.mul(h);
            dy2_ = dy2.mul(h);
            dxy_ = dxy.mul(h);
            a = cv::sum(dx2_)[0];
            d = cv::sum(dy2_)[0];
            b = cv::sum(dxy_)[0];
            c = b;
            Eigen::Matrix2d M;
            M << a, b,
                    c, d;
            double score = M.determinant() - 0.04 * (M.trace() * M.trace());

        }else{
            vSurfOff.at<double>(y,x) = ts;
            img.at<cv::Vec3b>(y,x)=cv::Vec3b(255,0,0);
            binaryOff.at<uchar>(y,x)=1;
            neg_queue.push_back(std::make_pair(x,y));

            if (pos_queue.size() == (NUM_EVENTS+1)) {
                std::pair<int, int> oldestPos = neg_queue.front();
                neg_queue.pop_front();
                img.at<cv::Vec3b>(oldestPos.second, oldestPos.first) = cv::Vec3b(0, 0, 0);
                vSurfOn.at<double>(oldestPos.second, oldestPos.first) = 0;
                binaryOn.at<uchar>(oldestPos.second, oldestPos.first) = 0;
            }

            cv::Rect ROI = cv::Rect(x - L, y - L, 2 * L + 1, 2 * L + 1);
            cv::Mat mat_roi = binaryOn(ROI);
            cv::Sobel(mat_roi, dx, CV_64F, 1, 0, 7, 0.01, cv::BORDER_CONSTANT);
            cv::Sobel(mat_roi, dy, CV_64F, 0, 1, 7, 0.01, cv::BORDER_CONSTANT);
            dx2 = dx.mul(dx);
            dy2 = dy.mul(dy);
            dxy = dx.mul(dy);
            cv::Mat h_ = cv::getGaussianKernel(2 * L + 1, 1);
            cv::Mat h = h_ * h_.t();

            dx2_ = dx2.mul(h);
            dy2_ = dy2.mul(h);
            dxy_ = dxy.mul(h);
            a = cv::sum(dx2_)[0];
            d = cv::sum(dy2_)[0];
            b = cv::sum(dxy_)[0];
            c = b;
            Eigen::Matrix2d M;
            M << a, b,
                    c, d;
            double score = M.determinant() - 0.04 * (M.trace() * M.trace());
        }
    }

    return;
}

int main(int argc, char* argv[]) {
    int lifetime = 10000;     // micro sec
    CornerDetection(argv[1], lifetime); // filename is specified with command line argument.
    return 0;
}
