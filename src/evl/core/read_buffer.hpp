/******************************************************************************/
/*! @addtogroup EVL_CORE
    @file       read_buffer.hpp
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

/*! @ingroup EVL_CORE */
/* @{ */

#ifndef SRC_EVL_CORE_READ_BUFFER_HPP_
#define SRC_EVL_CORE_READ_BUFFER_HPP_

#include <vector>

#include "types.hpp"

namespace evl {

/******************************************************************************/
/*! @brief Read buffered events based on lifetime.
    Reading event data based on lifetime timestamp.

    @param[in]      *buffer (EventBuffer) Buffer to read events from.
    @param[in]      lifetime    (int) Lifetime of events to read.

    @return         event_tuple (std::vector<EventTuple>) Vector of EventTuples.
    @exception      none    
******************************************************************************/
std::vector<EventTuple> readBufferOnLifetime(EventBuffer *buffer, int lifetime);

/******************************************************************************/
/*! @brief Read buffered events based on number of events.
    Reading event data based on how many events to read.

    @param[in]      *buffer (EventBuffer) Buffer to read events from.
    @param[in]      number  (int) Number of events to read.

    @return         event_tuple (std::vector<EventTuple>) Vector of EventTuples.
    @exception      none    
******************************************************************************/
std::vector<EventTuple> readBufferOnNumber(EventBuffer *buffer, int number);
}  // namespace evl

#endif  // SRC_EVL_CORE_READ_BUFFER_HPP_
