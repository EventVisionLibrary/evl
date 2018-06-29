// Copyright 2018 Event Vision Library.
#ifndef SRC_EVL_CORE_SHUTDOWN_HPP_
#define SRC_EVL_CORE_SHUTDOWN_HPP_

#include <atomic>

class Shutdown {
 public:
    static std::atomic_bool globalShutdown;

    static void globalShutdownSignalHandler(int signal);

    static void usbShutdownHandler(void *ptr);

    int setSigHandler();
};

#endif    // SRC_EVL_CORE_SHUTDOWN_HPP_
