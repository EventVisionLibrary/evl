#ifndef EVL_CORE_DAVISIO_hpp_
#define EVL_CORE_DAVISIO_hpp_

#include <mutex>
#include <vector>
#include <libcaercpp/libcaer.hpp>
#include <libcaercpp/devices/davis.hpp>

#include "types.hpp"

extern std::mutex mtx;

void disp(EventTuple x );

libcaer::devices::davis initializeDavis(void);

void bufferData(EventBuffer *buffer);

std::vector<EventTuple> readBufferOnLifetime(EventBuffer *buffer, int lifetime);

std::vector<EventTuple> readBufferOnNumber(EventBuffer *buffer, int number);

void loop_readData(EventBuffer *buffer, int lifetime);

#endif // EVL_CORE_DAVISIO_hpp_