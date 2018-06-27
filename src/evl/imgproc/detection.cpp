// Copyright 2018 Event Vision Library.
#include "detection.hpp"
#include <time.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../core/types.hpp"


void detect_rod_tip(std::vector<EventTuple> v, cv::Rect *roi) {
    // capable move pixel
    int dpx = 10;
    // for next roi
    int min_x = W;
    int min_y = H;
    int max_x = 0;
    int max_y = 0;

    // dot list used for lsq
    std::vector<int> X;
    std::vector<int> Y;
    cv::Mat src = cv::Mat::zeros(H, W, CV_8UC1);
    for (auto iter = v.begin(); iter != v.end(); ++iter) {
        int x = std::get<1>(*iter);
        int y = std::get<2>(*iter);
        src.at<unsigned char>(y, x) = 255;  // pollは1と仮定
        if (x >= (*roi).x - dpx && x <= (*roi).x + (*roi).width + dpx &&
            y >= (*roi).y - dpx && y <= (*roi).y+(*roi).height + dpx) {
            X.push_back(x);
            Y.push_back(y);
            if (x < min_x) {
                min_x = x;
            } else if (x > max_x) {
                max_x = x;
            }
            if (y < min_y) {
                min_y = y;
            } else if (y > max_y) {
                max_y = y;
            }
        }
    }

    (*roi).x = min_x;
    (*roi).y = min_y;
    (*roi).width = max_x - min_x;
    (*roi).height = max_y - min_y;
    cv::rectangle(src, *roi, 255, 2);

    cv::imshow("src", src);
    cv::waitKey(100);
    cv::destroyAllWindows();
    return;
}

void detect_rod_tip(std::vector<EventTuple> v,
  cv::Rect *roi, cv::Point *vertex) {
    // capable move pixel
    int dpx = 50;
    // for next roi
    int min_x = W;
    int min_y = H;
    int max_x = 0;
    int max_y = 0;
    int vertex_x = 0;
    int vertex_y = 0;

    cv::Mat src = cv::Mat::zeros(H, W, CV_8UC1);
    cv::Mat filtered = cv::Mat::zeros(H, W, CV_8UC1);

    for (auto iter = v.begin(); iter != v.end(); ++iter) {
        int x = std::get<1>(*iter);
        int y = std::get<2>(*iter);
        src.at<unsigned char>(y, x) = 255;   // pollは1と仮定
    }
    cv::medianBlur(src, filtered, 5);

    for (int y = 0; y < filtered.rows; ++y) {
        for (int x = 0; x < filtered.cols; ++x) {
            if (filtered.at<unsigned char>(y, x) == 255) {
                if (x >= (*roi).x - dpx && x <= (*roi).x + (*roi).width + dpx &&
                y >= (*roi).y - dpx && y <= (*roi).y + (*roi).height + dpx) {
                    if (x < min_x) {
                        min_x = x;
                    } else if (x > max_x) {
                        max_x = x;
                    }
                    if (y < min_y) {
                        min_y = y;
                        vertex_x = x;
                        vertex_y = y;
                    } else if (y > max_y) {
                        max_y = y;
                    }
                }
            }
        }
    }

    // calibration
    if (max_x < min_x || max_y < min_y || vertex_x == 0) {
        // if missing rod
        // vertex_x = W/2-10;
        // min_x = W/2-10;
        // min_y = H/2;
        // max_x = W/2+10;
        // max_y = H;
        min_x = (*roi).x;
        min_y = (*roi).y;
        max_x = min_x + (*roi).width;
        max_y = min_y + (*roi).height;
        vertex_x = (*vertex).x;
        vertex_y = (*vertex).y;
    }

    (*roi).x = min_x;
    (*roi).y = min_y;
    (*roi).width = max_x - min_x;
    (*roi).height = max_y - min_y;
    (*vertex).x = vertex_x;
    (*vertex).y = vertex_y;

    // cv::rectangle(src, *roi, 255, 2);
    // cv::rectangle(filtered, *roi, 255, 2);
    return;
}

void load_csv(std::vector<EventTuple> *v, int i) {
    FILE *fp;
    int ret;
    char fname[1000];
    snprintf(fname, "../../../sample_data/tsue_batch/%d.csv", i);
    fp = fopen(fname, "r");
    if (fp == NULL) {
        printf("%sThe file cannot be opened!\n", fname);
        return;
    }

    uint32_t ts; uint16_t x; uint16_t y;
    int pol_raw; bool pol;
    while ((ret = fscanf(fp, "%u,%hu,%hu,%d", &ts, &x, &y, &pol_raw)) != EOF) {
        pol = static_cast<bool>(pol_raw);
        (*v).push_back(std::make_tuple(ts, x, y, pol));
    }
    fclose(fp);
}

// int main()
// {
//    cv::Rect roi(50, 100, 150, 80); //(x, y, w, h), 最初だけ決め打ち
//    for (int i=0; i<50; i++) {
//        std::vector<EventTuple> v;
//        load_csv(&v, i);
//        detect_rod_tip(v, &roi);
//    }
//    return 1;
// }
