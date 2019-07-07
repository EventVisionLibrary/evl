/******************************************************************************/
/*! @addtogroup EVL_CORE
    @file       types.hpp
    @brief      Type definitions for EVL.
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

#ifndef SRC_EVL_CORE_TYPES_HPP_
#define SRC_EVL_CORE_TYPES_HPP_

#include <tuple>

#include <boost/circular_buffer.hpp>
#include <libcaercpp/libcaer.hpp>
#include <libcaercpp/devices/davis.hpp>

/*! @namespace  evl
    @brief Base namespace.
    
    Base namespce for this library.
*/
namespace evl
{

/*! @var   EventTuple
    @brief Tuple of events
    
    Tuple of <int32_t timestamp, uint16_t x, uint16_t y, bool polarity>.
*/
typedef std::tuple<double, uint16_t, uint16_t, bool> EventTuple;

/*! @var   EventBuffer
    @brief Buffer of events, using boost::circular_buffer.

    This variable uses boost::circular_buffer.
    @todo(shiba)  more details here...  
*/
typedef boost::circular_buffer<EventTuple> EventBuffer;

/*! @var   EventPolarity
    @brief Polarity of events, using libcaer.

    Polarity means increase (+1) of decrease (0) of events.
*/
typedef libcaer::events::PolarityEventPacket EventPolarity;

/*! @var   Davis
    @brief Davis camera instance.
*/
typedef libcaer::devices::davis Davis;
}  // namespace evl

#endif   // SRC_EVL_CORE_TYPES_HPP_
