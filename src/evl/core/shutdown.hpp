/******************************************************************************/
/*! @addtogroup EVL_CORE
    @file       shutdown.hpp
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

#ifndef SRC_EVL_CORE_SHUTDOWN_HPP_
#define SRC_EVL_CORE_SHUTDOWN_HPP_

#include <atomic>

namespace evl {

/*! @class Shutdown
    @brief  Shutdown handler of events.
    Handling SIGINT when pushed CTRL+C.
*/
class Shutdown {
  public:
    /*! @var   globalShutdown
    @brief Shutdown signal
    
    When get SIGINT, this variable is 1.
    */
    static std::atomic_bool globalShutdown;
    static void globalShutdownSignalHandler(int signal);
    static void usbShutdownHandler(void *ptr);
    int setSigHandler();
};
}  // namespace evl

#endif    // SRC_EVL_CORE_SHUTDOWN_HPP_
