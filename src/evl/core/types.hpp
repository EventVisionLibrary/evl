#ifndef EVL_CORE_TYPES_hpp_
#define EVL_CORE_TYPES_hpp_

#include <tuple>
#include <boost/circular_buffer.hpp>

typedef std::tuple<int32_t, uint16_t, uint16_t, bool> EventTuple;
typedef boost::circular_buffer<EventTuple> EventBuffer;

#endif // EVL_CORE_TYPES_hpp_
