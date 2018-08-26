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
void loadEvents(std::string filename, std::vector<Event>& events, bool skip_events, bool flip_ud);

void saveState(std::string filename, const cv::Mat *mat, bool as_png, bool as_npy);
void loadState(std::string filename, cv::Mat *mat, float u_min);

#endif  // COMMON_H
