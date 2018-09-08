/******************************************************************************/
/*! @addtogroup EVL_CORE
    @file       save_csv.hpp
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

#ifndef SRC_EVL_CORE_SAVE_CSV_HPP_
#define SRC_EVL_CORE_SAVE_CSV_HPP_

#include <string>
#include "types.hpp"

namespace evl {

/******************************************************************************/
/*! @brief Save buffered events to file.
    Saving event data into csv file.

    @param[in]      *buffer (EventBuffer) Buffer which stores events.
    @param[in]      lifetime    (int) Lifetime of events to read.
    @param[in]      filename    (std::string) Filename to save event data (.csv or .txt).

    @exception      none    
******************************************************************************/
void saveBuffer(EventBuffer *buffer, int lifetime, std::string filename);
}  // namespace evl

#endif  // SRC_EVL_CORE_SAVE_CSV_HPP_
