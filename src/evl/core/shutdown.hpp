// Copyright 2018 Event Vision Library.
#ifndef SRC_EVL_CORE_SHUTDOWN_HPP_
#define SRC_EVL_CORE_SHUTDOWN_HPP_

#include <atomic>

namespace evl {
  class Shutdown {
   public:
      static std::atomic_bool globalShutdown;

      static void globalShutdownSignalHandler(int signal);

      static void usbShutdownHandler(void *ptr);

      int setSigHandler();
  };
}  // namespace evl

#endif    // SRC_EVL_CORE_SHUTDOWN_HPP_
