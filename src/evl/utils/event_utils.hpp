/******************************************************************************/
/*! @addtogroup EVL_UTILS
    @file       event_utils.hpp
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

/*! @ingroup EVL_UTILS */
/* @{ */

#ifndef SRC_EVL_UTILS_EVENT_UTILS_HPP_
#define SRC_EVL_UTILS_EVENT_UTILS_HPP_

#include <vector>

#include <opencv2/core/core.hpp>
#include <evl/core/types.hpp>

namespace evl {

/******************************************************************************/
/*! @brief Print events to stdout.

    @param[in]      events      Events to print.
    @param[in]      with_pol    It True, print events with polarity.

    @exception      none
******************************************************************************/
void printEvent(EventTuple x);

/******************************************************************************/
/*! @brief Convert events to matrix (opencv)

    @param[in]      events      Events to print.
    @param[in]      with_pol    If true, convert events with polarity. (returns TYPE)
                                If false, returns TYPE.

    @return         OpenCV matrix.
    @exception      none
******************************************************************************/
cv::Mat convertEventsToMat(std::vector<EventTuple> events, bool with_pol = false);
}  // namespace evl

#endif    // SRC_EVL_UTILS_EVENT_UTILS_HPP_
