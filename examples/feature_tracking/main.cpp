// Copyright 2018 Event Vision Library.
#include <iostream>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cvaux.h>

#include <evl/core/types.hpp>
#include <evl/core/buffer_csv.hpp>
#include <evl/core/read_buffer.hpp>
#include <evl/utils/event_utils.hpp>

#include <queue>



/**
 * @fn void CornerDetection(char* fname, int lifetime)
 * @brief Offline Harris corner detection.
 * @param (fname) .csv file name.
 * @param (lifetime) event lifetime for image projection.
 */
void CornerDetection(char* fname, int lifetime){

    //Reading all event data from csv and save it to que.
    std::vector<evl::EventTuple> v;
    std::queue<evl::EventTuple> que;
    FILE *fp;
    int32_t start_time;
    int32_t current_time;
    cv::Mat img, harris_img, gray_img;
    std::vector <cv::Point2f> corners;


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
        que.push(tup);
    }
    fclose(fp);




    while(!que.empty()) {
        //Create image from events.
        start_time = std::get<0>(que.front());
        current_time = start_time;
        while (!que.empty() and current_time < start_time + lifetime) {
            v.push_back(que.front());
            current_time = std::get<0>(que.front());
            que.pop();
        }
        img = evl::events_to_mat(v, true);

        //Harris corner detection
        harris_img = img.clone();
        cv::cvtColor(img, gray_img, CV_BGR2GRAY);
        cv::normalize(gray_img, gray_img, 0, 255, cv::NORM_MINMAX);

        cv::goodFeaturesToTrack(gray_img, corners, 80, 0.01, 3, cv::Mat(), 3, true);
        std::vector<cv::Point2f>::iterator it_corner = corners.begin();
        for (; it_corner != corners.end(); ++it_corner) {
            circle(harris_img, cv::Point(it_corner->x, it_corner->y), 1, cv::Scalar(0, 255, 0), -1);
            circle(harris_img, cv::Point(it_corner->x, it_corner->y), 8, cv::Scalar(0, 255, 0));
        }

        //free memory
        std::vector<evl::EventTuple>().swap(v); //free memory

        //Visualization
        cv::imshow("orig", img);
        cv::imshow("harris", harris_img);
        cv::waitKey(50); //Note: This "50" is a magic number to show the video stream like real-time.
    }

    return;
}

int main(int argc, char* argv[]) {
    int lifetime = 10000;     // micro sec
    CornerDetection(argv[1], lifetime); // filename is specified with command line argument.
    return 0;
}
