#ifndef EVL_IMPROC_DETECTION_hpp_
#define EVL_IMPROC_DETECTION_hpp_

#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#define W 240
#define H 180

void detect_rod_tip(std::vector<EventTuple>, cv::Rect *roi);
void detect_rod_tip(std::vector<EventTuple> v, cv::Rect *roi, cv::Point *vertex);

#endif // EVL_IMPROC_DETECTION_hpp_
