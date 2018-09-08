/******************************************************************************/
/*! @addtogroup EVL_CORE
    @file       initialize_davis.hpp
    @brief      Initializing DAVIS.
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

#ifndef SRC_EVL_CORE_INITIALIZE_DAVIS_HPP_
#define SRC_EVL_CORE_INITIALIZE_DAVIS_HPP_

#include "types.hpp"

    namespace evl {

/******************************************************************************/
/*! @brief Initialize DAVIS before reading data.

    Configuring several parameters and setting up ready to read DAVIS.
    @return         Success 1, failure 0
    @exception      Raise error when there is no DAVIS connected.
    @todo(shiba)    Input argument as DAVIS ID (default=1).
******************************************************************************/
Davis initializeDavis(void);
}  // namespace evl

#endif  // SRC_EVL_CORE_INITIALIZE_DAVIS_HPP_
