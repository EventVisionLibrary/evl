#ifndef DENOISE_H
#define DENOISE_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

enum METHOD {
    TV_L1, TV_L2, TV_LogL2, TV_KLD,
};

namespace cuda {

    void initDenoise(cv::CV_32FC1 *u, cv::CV_32FC1 *timestamp);
    void solveTVIncrementalManifold(cv::CV_32FC1 *u, cv::CV_32FC1 *f, cv::CV_32FC1 *t,
                                    float lambda, float lambda_time,
                                    int iterations, float u_min, float u_max, METHOD method);
}  // namespace cuda

#endif
