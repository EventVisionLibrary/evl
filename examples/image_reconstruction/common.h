#ifndef COMMON_H
#define COMMON_H
#include "event.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <string>
#include <vector>

#define TIME_CONSTANT 1e-6f

// IO functions
void saveEvents(std::string filename, std::vector<Event> &events);
// void saveState(std::string filename, const cv::CV_32FC1 *mat, bool as_png=false, bool as_npy=false);
void saveState(std::string filename, const cv::Mat *mat, bool as_png=false, bool as_npy=false);
void loadEvents(std::string filename, std::vector<Event>& events, bool skip_events=false, bool flip_ud=false);
// void loadState(std::string filename, cv::CV_32FC1 *mat, float u_min);
void loadState(std::string filename, cv::Mat *mat, float u_min);

#endif  // COMMON_H
