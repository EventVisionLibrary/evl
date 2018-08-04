// Copyright 2018 Event Vision Library.
#include "shutdown.hpp"

#include <atomic>
#include <csignal>
#include <cstddef>
#include <libcaercpp/libcaer.hpp>

namespace evl {
  std::atomic_bool Shutdown::globalShutdown(false);

  void Shutdown::globalShutdownSignalHandler(int signal) {
      // Simply set the running flag to false
      // on SIGTERM and SIGINT (CTRL+C) for global shutdown.
      if (signal == SIGTERM || signal == SIGINT) {
          Shutdown::globalShutdown.store(true);
      }
  }

  void Shutdown::usbShutdownHandler(void *ptr) {
      (void)(ptr);      // UNUSED.
      Shutdown::globalShutdown.store(true);
  }

  int Shutdown::setSigHandler() {
  // Install signal handler for global shutdown.
  #if defined(_WIN32)
      if (signal(SIGTERM, globalShutdownSignalHandler) == SIG_ERR) {
          libcaer::log::log(libcaer::log::logLevel::CRITICAL, "ShutdownAction",
              "Failed to set signal handler for SIGTERM. Error: %d.", errno);
          return (EXIT_FAILURE);
      }

      if (signal(SIGINT, globalShutdownSignalHandler) == SIG_ERR) {
          libcaer::log::log(libcaer::log::logLevel::CRITICAL, "ShutdownAction",
              "Failed to set signal handler for SIGINT. Error: %d.", errno);
          return (EXIT_FAILURE);
      }
  #else
      struct sigaction shutdownAction;

      shutdownAction.sa_handler = &globalShutdownSignalHandler;
      shutdownAction.sa_flags = 0;
      sigemptyset(&shutdownAction.sa_mask);
      sigaddset(&shutdownAction.sa_mask, SIGTERM);
      sigaddset(&shutdownAction.sa_mask, SIGINT);

      if (sigaction(SIGTERM, &shutdownAction, NULL) == -1) {
          libcaer::log::log(libcaer::log::logLevel::CRITICAL, "ShutdownAction",
              "Failed to set signal handler for SIGTERM. Error: %d.", errno);
          return (EXIT_FAILURE);
      }

      if (sigaction(SIGINT, &shutdownAction, NULL) == -1) {
          libcaer::log::log(libcaer::log::logLevel::CRITICAL, "ShutdownAction",
              "Failed to set signal handler for SIGINT. Error: %d.", errno);
          return (EXIT_FAILURE);
      }
  #endif
      return (EXIT_FAILURE);
  }
}  // namespace evl
