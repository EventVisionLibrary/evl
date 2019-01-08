
/*
Copyright 2018 Event Vision Library.

Implementation of "Fast Event-based Harris Corner Detection Exploiting the Advantages of Event-driven Cameras",
 https://www.semanticscholar.org/paper/Fast-event-based-Harris-corner-detection-exploiting-Vasco-Glover/2d57a3b949a82e2ff9d6dc37d0a53e8c1514af22
*/
#include <iostream>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

#include <evl/core/types.hpp>
#include <evl/core/buffer_csv.hpp>
#include <evl/core/read_buffer.hpp>
#include <evl/utils/event_utils.hpp>

#include <queue>
#include <Eigen/Core>
#include <Eigen/Geometry>

#define W 240
#define H 180
#define L 7
#define NUM_EVENTS 2000
#define POSITIVE true
#define NEGATIVE false



void CornerDetection(char* fname, int lifetime){
    //Reading all event data from csv and save it to queue.
    std::queue<evl::EventTuple> queue;
    FILE *fp;
    cv::Mat img;
    cv::Mat vSurfOn, vSurfOff, binaryOn, binaryOff;
    cv::Mat Gx, Gy, dx, dy, dx2, dy2, dxy, dx2_, dy2_, dxy_;
    cv::Point minLoc;
    double minVal, maxVal, a, b, c, d;

    std::vector <cv::Point2f> corners;
    int N_pos=0, N_neg=0;

    img = cv::Mat::zeros(H, W, CV_8UC3);
    vSurfOn = cv::Mat::zeros(H, W, CV_64FC1);
    vSurfOff = cv::Mat::zeros(H, W, CV_64FC1);
    binaryOn = cv::Mat::zeros(H, W, CV_8UC1);
    binaryOff = cv::Mat::zeros(H, W, CV_8UC1);

    cv::Mat kx, ky, h;
    cv::getDerivKernels(kx, ky, 1, 0, L, false, CV_32F);
    Gx = ky * kx.t();
    cv::minMaxLoc(Gx, NULL, &maxVal, NULL, NULL);
    Gx /= maxVal;
    Gy = Gx.t();

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
    std::vector<evl::EventTuple> ev;
    while ((ret=fscanf(fp, "%lf,%lf,%lf,%lf", &ts, &x, &y, &pol_raw)) != EOF and ev.size() < 8000) {
        pol = static_cast<bool>(pol_raw);
        evl::EventTuple tup = std::make_tuple(ts, x, y, pol);
        ev.push_back(tup);
    }
    fclose(fp);

    int count=0;

    //Create image from events.
    for(int i=0; i<4000; i++) {
        count++;
        evl::EventTuple event_cur = ev[i];
        ts = std::get<0>(event_cur);
        x = std::get<1>(event_cur);
        y = std::get<2>(event_cur);
        pol = std::get<3>(event_cur);

        if (x <= L or x >= W - L or y <= L or y >= H - L)
            continue;

        if (pol == POSITIVE) {
            std::cout << "hi " << std::endl;
            vSurfOn.at<double>(y, x) = ts;
            img.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
            binaryOn.at<uchar>(y, x) = 1;
            N_pos++;
            if (N_pos > NUM_EVENTS) {
                cv::minMaxLoc(vSurfOn, &minVal, NULL, &minLoc, NULL, binaryOn);
                img.at<cv::Vec3b>(minLoc.y, minLoc.x) = cv::Vec3b(0, 0, 0);
                vSurfOn.at<double>(minLoc.y, minLoc.x) = 0;
                binaryOn.at<uchar>(minLoc.y, minLoc.x) = 0;
                N_pos--;
            }

            cv::Rect ROI = cv::Rect(x - L, y - L, 2 * L + 1, 2 * L + 1);
            cv::Mat mat_roi = binaryOn(ROI);
            cv::Sobel(mat_roi, dx, CV_64F, 1, 0, 7, 0.01, cv::BORDER_CONSTANT);
            cv::Sobel(mat_roi, dy, CV_64F, 0, 1, 7, 0.01, cv::BORDER_CONSTANT);
            dx2 = dx.mul(dx);
            dy2 = dy.mul(dy);
            dxy = dx.mul(dy);
            cv::Mat h_ = cv::getGaussianKernel(15, 1);
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
            if(count == 4000) {
                cv::Mat show = dx;
                std::cout.precision(3);
                for (int y = 3; y < show.rows-3; ++y) {
                    for (int x = 3; x < show.cols-3; ++x) {
                        std::cout << dy.at<double>(y, x) << " ";
                    }
                    std::cout<<std::endl;
                }
                std::cout<<std::endl;
                std::cout << ts << std::endl;
                std::cout<<M<<std::endl;
                std::cout<<score<<std::endl;
            }

        } else {
            std::cout << "hi2 " << std::endl;

            vSurfOff.at<double>(y, x) = ts;
            img.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 0, 0);
            binaryOff.at<uchar>(y, x) = 1;
            N_neg++;
            if (N_neg > NUM_EVENTS) {
                cv::minMaxLoc(vSurfOff, &minVal, NULL, &minLoc, NULL, binaryOff);
                img.at<cv::Vec3b>(minLoc.y, minLoc.x) = cv::Vec3b(0, 0, 0);
                vSurfOff.at<double>(minLoc.y, minLoc.x) = 0;
                binaryOff.at<uchar>(minLoc.y, minLoc.x) = 0;
                N_neg--;
            }

            cv::Rect ROI = cv::Rect(x - L, y - L, 2 * L + 1, 2 * L + 1);
            cv::Mat mat_roi = binaryOff(ROI);
            cv::Sobel(mat_roi, dx, CV_64F, 1, 0, 7, 0.01, cv::BORDER_CONSTANT);
            cv::Sobel(mat_roi, dy, CV_64F, 0, 1, 7, 0.01, cv::BORDER_CONSTANT);


            dx2 = dx.mul(dx);
            dy2 = dy.mul(dy);
            dxy = dx.mul(dy);

//            std::cout << count << std::endl;
//            std::cout << dx << std::endl;

            //std::cout<<dx2<<std::endl;
            //std::cout<<"debug "<< dx2*h << std::endl;
            cv::GaussianBlur(dx2, dx2_, dx2.size(), 1, 1, 0);
            cv::GaussianBlur(dy2, dy2_, dy2.size(), 1, 1, 0);
            cv::GaussianBlur(dxy, dxy_, dxy.size(), 1, 1, 0);

            a = cv::sum(dx2_)[0];
            d = cv::sum(dy2_)[0];
            b = cv::sum(dxy_)[0];
            c = b;
            Eigen::Matrix2d M;
            M << a, b,
                    c, d;

            double score = M.determinant() - 0.04 * pow(M.trace(), 2);
        }

    }
//
//        //Visualization
        //cv::imshow("orig", img);
//        cv::imshow("harris", harris_img);
        //cv::waitKey(50); //Note: This "50" is a magic number to show the video stream like real-time.


    return;
}

int main(int argc, char* argv[]) {
    int lifetime = 10000;     // micro sec
    CornerDetection(argv[1], lifetime); // filename is specified with command line argument.
    return 0;
}
