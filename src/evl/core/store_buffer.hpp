/******************************************************************************/
/*! @addtogroup EVL_CORE
    @file       store_buffer.hpp
    @brief      Store buffer of event data.
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

#ifndef SRC_EVL_CORE_STORE_BUFFER_HPP_
#define SRC_EVL_CORE_STORE_BUFFER_HPP_

#include "types.hpp"

namespace evl {

/******************************************************************************/
/*! @brief Store events from CSV file to buffer.
    Store events from CSV file to buffer.
    Offline reader.

    @param[in]      *buffer (EventBuffer) Buffer to store events.
    @param[in]      *fname  (char) Filename of saved events (.csv or .txt).

    @exception      none    
******************************************************************************/
void storeBufferFromCsv(EventBuffer *buffer, char *fname);

/******************************************************************************/
/*! @brief Store events from DAVIS to buffer.
    Store events from DAVIS to buffer.
    Online reader.

    @param[in]      *buffer (EventBuffer) Buffer to store events.

    @exception      none    
******************************************************************************/
void storeBufferFromDavis(EventBuffer *buffer);
}  // namespace evl

#endif  // SRC_EVL_CORE_STORE_BUFFER_HPP_
