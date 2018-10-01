/******************************************************************************/
/*! @addtogroup EVL_CORE
    @file       common.hpp
    @brief      Common header file for global mutex variable.
*******************************************************************************
    Detail      We have "mutex" in common for multi threading.
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

#ifndef SRC_EVL_CORE_COMMON_HPP_
#define SRC_EVL_CORE_COMMON_HPP_

#include <mutex>

/*! @var   mutex
    @brief Global variable inside this library.

    mutex lock for multi threading.

*/
extern std::mutex mtx;

#endif   // SRC_EVL_CORE_COMMON_HPP_
