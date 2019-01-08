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

#include "params.h"

#include <Eigen/Dense>

#define INIT 100
/**
 * @fn void CornerDetection(char* fname, int lifetime)
 * @brief Offline Harris corner detection.
 * @param (fname) .csv file name.
 * @param (lifetime) event lifetime for image projection.
 */
void CornerDetection(char* fname, int lifetime){

    std::vector<evl::EventTuple> events;
    std::queue<evl::EventTuple> que;
    FILE *fp;
    int32_t start_time;
    int32_t current_time;
    cv::Mat img, harris_img, gray_img;
    std::vector <cv::Point2f> corners;
    int ret;
    double ts; double x; double y;
    double pol_raw; bool pol;

    double flow[2] = {0, 0};


    //Reading all event data from csv and save it to que.
    fp = fopen(fname, "r");
    if (fp == NULL) {
        printf("%sThe file cannot be opened!\n", fname);
        return;
    }
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
            events.push_back(que.front());
            current_time = std::get<0>(que.front());
            que.pop();
        }
        img = evl::events_to_mat(events, true);

        int size = events.size();
        Eigen::MatrixXd event_mat = Eigen::MatrixXd::Zero(size, 4);
        for(int i=0; i<size; i++){
            event_mat(i,0) = std::get<0>(events[i]);
            event_mat(i,1) = std::get<1>(events[i]);
            event_mat(i,2) = std::get<2>(events[i]);
            event_mat(i,3) = std::get<3>(events[i]);

        }

        //Harris corner detection
        harris_img = img.clone();
        cv::cvtColor(img, gray_img, CV_BGR2GRAY);
        cv::normalize(gray_img, gray_img, 0, 255, cv::NORM_MINMAX);

        cv::goodFeaturesToTrack(gray_img, corners, 80, 0.01, 3, cv::Mat(), 3, true);
        std::vector<cv::Point2f>::iterator it_corner = corners.begin();

        int target_time = std::get<0>(*events.begin());

        for (; it_corner != corners.end(); ++it_corner) {
            circle(harris_img, cv::Point(it_corner->x, it_corner->y), 1, cv::Scalar(0, 255, 0), -1);
            circle(harris_img, cv::Point(it_corner->x, it_corner->y), 8, cv::Scalar(0, 255, 0));

            ///EM1
//            std::vector<evl::EventTuple> centered_events(size);
//            for(auto x : events){
//                centered_events.push_back(std::make_tuple(std::get<0>(x), std::get<1>(x)-it_corner->x,
//                                                          std::get<2>(x)-it_corner->y, std::get<3>(x)));
//            }

            Eigen::MatrixXd centered_events_mat = event_mat;
            for(int i=0; i<size; i++){
                centered_events_mat(i,1) -= it_corner->x;
                centered_events_mat(i,2) -= it_corner->y;
            }

//            std::vector<bool> event_window(size);
            Eigen::VectorXd event_window_mat = Eigen::VectorXd::Constant(size,INIT); //

            int itr=0;
            while(true){
                if(itr >= max_interation)
                    break;

//                std::vector<evl::EventTuple> time_shifted_points(size);
//                for(auto x : centered_events){
//                    time_shifted_points.push_back(std::make_tuple(std::get<0>(x), std::get<1>(x)+flow[0]*(target_time - std::get<0>(x)),
//                                                                  std::get<2>(x)+flow[1]*(target_time - std::get<0>(x)), std::get<3>(x)));
//                }

                Eigen::MatrixXd time_shifted_points_mat = centered_events_mat;
                for(int i=0; i<size; i++){
                    time_shifted_points_mat(i,1) += flow[0]*(target_time-time_shifted_points_mat(i,0));
                    time_shifted_points_mat(i,2) += flow[1]*(target_time-time_shifted_points_mat(i,0));
                }


                if(event_window_mat(0) == INIT){
                    for(int i=0; i<size; i++){
//                        event_window[i] = ((std::get<1>((time_shifted_points[i]))) > -window_size/2 and
//                                (std::get<2>((time_shifted_points[i]))) > -window_size/2 and
//                                (std::get<2>((time_shifted_points[i]))) < window_size/2 and
//                                (std::get<2>((time_shifted_points[i]))) < window_size/2 );

                        event_window_mat(i) = ((std::get<1>((time_shifted_points[i]))) > -window_size/2 and
                                               (std::get<2>((time_shifted_points[i]))) > -window_size/2 and
                                               (std::get<2>((time_shifted_points[i]))) < window_size/2 and
                                               (std::get<2>((time_shifted_points[i]))) < window_size/2 );

                        centered_events[i] = ((event_window[i]) ? centered_events[i] : std::make_tuple(0, (short unsigned int)0, (short unsigned int)0, false));
                        time_shifted_points[i] = ((event_window[i]) ? time_shifted_points[i] : std::make_tuple(0, (short unsigned int)0, (short unsigned int)0, false));

                    }
                }

//                std::vector<evl::EventTuple> normalized_events = time_shifted_points;
//                for(int i=0; i<size; i++){
//
//                }

                itr++;
            }



        }


        //free memory
        std::vector<evl::EventTuple>().swap(events);


        //Visualization
        cv::imshow("orig", img);
        cv::imshow("harris", harris_img);
        cv::waitKey(50); //Note: This "50" is a magic number to show the video stream like real-time.
    }

    return;
}

//Usage: ./feature_tracking_bin ../data/8.csv
int main(int argc, char* argv[]) {
    int lifetime = 10000;     // micro sec
    CornerDetection(argv[1], lifetime); // filename is specified with command line argument.
    return 0;
}
