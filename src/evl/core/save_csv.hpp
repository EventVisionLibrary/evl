// Copyright 2018 Event Vision Library

#ifndef SRC_EVL_CORE_SAVE_CSV_HPP_
#define SRC_EVL_CORE_SAVE_CSV_HPP_

#include <string>
#include "types.hpp"

namespace evl {
  void loop_saveData(EventBuffer *buffer, int lifetime, std::string filename);
}  // namespace evl

#endif  // SRC_EVL_CORE_SAVE_CSV_HPP_
