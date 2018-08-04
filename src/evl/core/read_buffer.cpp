// Copyright 2018 Event Vision Library.
#include <vector>
#include <mutex>
#include "unistd.h"     //  for sleep function

#include "types.hpp"
#include "shutdown.hpp"
#include "common.hpp"

#include "read_buffer.hpp"

std::mutex mtx;


namespace evl {
  std::vector<EventTuple> readBufferOnLifetime(EventBuffer *buffer,
    int lifetime) {
      std::vector<EventTuple> v;

      mtx.lock();
      EventTuple tup = (*buffer).front();    // get first element
      int32_t starttime = std::get<0>(tup);
      (*buffer).pop_front();                  // remove first element
      v.push_back(tup);

      for (int cnt = 0; cnt < (*buffer).capacity(); cnt++) {
          EventTuple tup = (*buffer).front();    // get first element
          if (std::get<0>(tup) == 0) {
            break;
          } else if (std::get<0>(tup) > starttime - lifetime) {
              (*buffer).pop_front();                  // remove first element
              v.push_back(tup);
          } else {break;}
      }
      mtx.unlock();
      return v;
  }

  std::vector<EventTuple> readBufferOnNumber(EventBuffer *buffer, int number) {
      std::vector<EventTuple> v;

      mtx.lock();
      EventTuple tup = (*buffer).front();    // get first element
      (*buffer).pop_front();                  // remove first element
      v.push_back(tup);

      for (int cnt = 0; cnt < (*buffer).capacity(); cnt++) {
          EventTuple tup = (*buffer).front();    // get first element
          if (std::get<0>(tup) == 0) {
            break;
          } else if (v.size() < number) {
              (*buffer).pop_front();                  // remove first element
              v.push_back(tup);
          } else {
            break;
          }
      }
      mtx.unlock();
      return v;
  }
}  // namespace evl
