// Copyright 2018 Event Vision Library.
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <deque>
#include <tuple>
#include "unistd.h"

#include <libcaercpp/libcaer.hpp>
#include <libcaercpp/devices/davis.hpp>
#include "initialize_davis.hpp"
#include "davisio.hpp"
#include "types.hpp"
#include "shutdown.hpp"

std::mutex mtx;

void disp(EventTuple x ) {
    std::cout << ' ' << std::get<0>(x) << ' ' << \
    std::get<1>(x) << ' ' << std::get<2>(x) << ' ' << \
    std::get<3>(x) << std::endl;
}

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
    int32_t starttime = std::get<0>(tup);
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

void loop_readData(EventBuffer *buffer, int lifetime) {
    usleep(100000);      // micro sec
    while (1) {
        usleep(10000);      // micro sec. calling frequency
        std::vector<EventTuple> v = readBufferOnLifetime(buffer, lifetime);

        std::cout << "[Thread2] DATA READING =============" << std::endl;
        std::cout << "[Thread2] Lifetime >>> " << lifetime << std::endl;
        std::for_each((v).begin(), (v).end(), disp);
        std::cout << "[Thread2] DATA READ DONE ===========" << std::endl;
    }
}
