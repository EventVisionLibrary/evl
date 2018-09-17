// Copyright 2018 Event Vision Library.
#ifndef SRC_EVL_CORE_STORE_BUFFER_HPP_
#define SRC_EVL_CORE_STORE_BUFFER_HPP_

#include "types.hpp"

namespace evl {
void storeBufferFromCsv(EventBuffer *buffer, char* fname);
void storeBufferFromDavis(EventBuffer *buffer);
}  // namespace evl

#endif  // SRC_EVL_CORE_STORE_BUFFER_HPP_