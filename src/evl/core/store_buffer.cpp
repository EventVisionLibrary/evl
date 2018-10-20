// Copyright 2018 Event Vision Library.
#include "store_buffer.hpp"

#include "unistd.h"
#include <fstream>
#include <iostream>
#include <tuple>
#include <memory>

#include <libcaercpp/libcaer.hpp>
#include "common.hpp"
#include "initialize_davis.hpp"
#include "shutdown.hpp"
#include "types.hpp"

namespace evl {
void storeBufferFromCsv(EventBuffer *buffer, char* fname) {
    FILE *fp;
    fp = fopen(fname, "r");

    if (fp == NULL) {
        printf("%sThe file cannot be opened!\n", fname);
        return;
    }
    int ret;
    // int32_t ts; uint16_t x; uint16_t y;
    // int pol_raw; bool pol;

    double ts; double x; double y;
    double pol_raw; bool pol;
    double before_time = 0;

    while ((ret=fscanf(fp, "%lf,%lf,%lf,%lf", &ts, &x, &y, &pol_raw)) != EOF) {
        if (before_time > 0) {
            usleep(ts - before_time);
        }
        pol = static_cast<bool>(pol_raw);
        EventTuple tup = std::make_tuple(ts, x, y, pol_raw);
        mtx.lock();
        (*buffer).push_front(tup);
        mtx.unlock();
        before_time = ts;
    }
    fclose(fp);
    std::cout << "[Thread1] Buffering Loop Finished!! " << std::endl;
}

void storeBufferFromDavis(EventBuffer *buffer) {
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
