// Copyright 2018 Event Vision Library.
#include <iostream>
#include <tuple>

#include <libcaercpp/libcaer.hpp>
#include "initialize_davis.hpp"
#include "buffer_davis.hpp"
#include "types.hpp"
#include "shutdown.hpp"
#include "common.hpp"

namespace evl {
  void bufferData(EventBuffer *buffer) {
      Davis davisHandle = initializeDavis();
      davisHandle.dataStart(nullptr, nullptr, nullptr,
        &Shutdown::usbShutdownHandler, nullptr);

      // Blocking data-get mode to avoid wasting resources.
      davisHandle.configSet(CAER_HOST_CONFIG_DATAEXCHANGE,
        CAER_HOST_CONFIG_DATAEXCHANGE_BLOCKING, true);
      while (!Shutdown::globalShutdown.load(std::memory_order_relaxed)) {
          std::unique_ptr<libcaer::events::EventPacketContainer> \
          packetContainer = davisHandle.dataGet();
          if (packetContainer == nullptr) {
              continue;  // Skip if nothing there.
          }

          // printf("\nGot event container with %d packets (allocated).\n",
          // packetContainer->size());
          for (auto &packet : *packetContainer) {
              if (packet == nullptr) {
                  continue;     // Skip if nothing there.
              }

              if (packet->getEventType() == POLARITY_EVENT) {
                  std::shared_ptr<const EventPolarity> polarity = \
                  std::static_pointer_cast<EventPolarity>(packet);

                  mtx.lock();
                  for (auto &event : *polarity) {
                      int32_t ts = event.getTimestamp();
                      uint16_t x = event.getX();
                      uint16_t y = event.getY();
                      bool pol = event.getPolarity();
                      EventTuple tup = std::make_tuple(ts, x, y, pol);
                      (*buffer).push_front(tup);
                  }
                  mtx.unlock();
              }
          }
      }
      std::cout << "[Thread1] Buffering Loop Finished!! " << std::endl;
      davisHandle.dataStop();
  }
}  // namespace evl
