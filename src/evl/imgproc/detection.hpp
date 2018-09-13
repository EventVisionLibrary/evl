/******************************************************************************/
/*! @addtogroup EVL_IMGPROC
    @file       detection.hpp
    @brief      
*******************************************************************************
    Detail      
*******************************************************************************
    @date       2018/09/01
    @author     Event Vision Library
    @par        Revision
    0.1
    @par        Copyright
    2018-2018   Event Vision Library. All rights reserved.
*******************************************************************************
    @par        History
    - 2018/09/01 shiba
      -# Initial Version
******************************************************************************/

/*! @ingroup EVL_IMGPROC */
/* @{ */ 

#ifndef SRC_EVL_IMGPROC_DETECTION_HPP_
#define SRC_EVL_IMGPROC_DETECTION_HPP_

#include <vector>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "evl/core/types.hpp"

/*! @name CamereResolution
    Camera resolution. Currently, these are global variables.

    @todo(shiba)  More flexible
*/
/* @{ */
#define W 240 /*!< Width */
#define H 180 /*!< Height */

namespace evl {

void detect_rod_tip(std::vector<EventTuple> v, cv::Rect *roi);
void detect_rod_tip(std::vector<EventTuple> v,
                    cv::Rect *roi, cv::Point *vertex);
}  // namespace evl

#endif    // SRC_EVL_IMGPROC_DETECTION_HPP_
