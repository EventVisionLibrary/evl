// Copyright 2018 Event Vision Library.
#include <evl/core/shutdown.hpp>
#include <libcaercpp/libcaer.hpp>
#include <libcaercpp/devices/davis.hpp>

int main(void) {
    // Open a DAVIS, give it a device ID of 1,
    // and don't care about USB bus or SN restrictions.
    libcaer::devices::davis davisHandle = libcaer::devices::davis(1);

    // Let's take a look at the information we have on the device.
    struct caer_davis_info davis_info = davisHandle.infoGet();

    printf("%s --- ID: %d, Master: %d, DVS X: %d, DVS Y: %d, Logic: %d.\n",
      davis_info.deviceString, davis_info.deviceID, davis_info.deviceIsMaster,
      davis_info.dvsSizeX, davis_info.dvsSizeY, davis_info.logicVersion);

    // Send the default configuration before using the device.
    // No configuration is sent automatically!
    davisHandle.sendDefaultConfig();

    // Tweak some biases, to increase bandwidth in this case.
    struct caer_bias_coarsefine coarseFineBias;

    coarseFineBias.coarseValue = 2;
    coarseFineBias.fineValue = 116;
    coarseFineBias.enabled = true;
    coarseFineBias.sexN = false;
    coarseFineBias.typeNormal = true;
    coarseFineBias.currentLevelNormal = true;

    davisHandle.configSet(DAVIS_CONFIG_BIAS, DAVIS240_CONFIG_BIAS_PRBP,
      caerBiasCoarseFineGenerate(coarseFineBias));

    coarseFineBias.coarseValue = 1;
    coarseFineBias.fineValue = 33;
    coarseFineBias.enabled = true;
    coarseFineBias.sexN = false;
    coarseFineBias.typeNormal = true;
    coarseFineBias.currentLevelNormal = true;

    davisHandle.configSet(DAVIS_CONFIG_BIAS, DAVIS240_CONFIG_BIAS_PRSFBP,
      caerBiasCoarseFineGenerate(coarseFineBias));

    // Let's verify they really changed!
    uint32_t prBias = davisHandle.configGet(DAVIS_CONFIG_BIAS,
      DAVIS240_CONFIG_BIAS_PRBP);
    uint32_t prsfBias = davisHandle.configGet(DAVIS_CONFIG_BIAS,
      DAVIS240_CONFIG_BIAS_PRSFBP);

    printf("New bias values --- PR-coarse: %d, PR-fine: %d, PRSF-coarse: %d, PRSF-fine: %d.\n",   // NOLINT
        caerBiasCoarseFineParse(prBias).coarseValue,
        caerBiasCoarseFineParse(prBias).fineValue,
        caerBiasCoarseFineParse(prsfBias).coarseValue,
        caerBiasCoarseFineParse(prsfBias).fineValue);

    davisHandle.dataStart(nullptr, nullptr, nullptr,
      &Shutdown::usbShutdownHandler, nullptr);

    // Let's turn on blocking data-get mode to avoid wasting resources.
    davisHandle.configSet(CAER_HOST_CONFIG_DATAEXCHANGE,
      CAER_HOST_CONFIG_DATAEXCHANGE_BLOCKING, true);

    while (!Shutdown::globalShutdown.load(std::memory_order_relaxed)) {
        std::unique_ptr<libcaer::events::EventPacketContainer> \
        packetContainer = davisHandle.dataGet();
        if (packetContainer == nullptr) {
            continue;     // Skip if nothing there.
        }

        printf("\nGot event container with %d packets (allocated).\n",
          packetContainer->size());

        for (auto &packet : *packetContainer) {
            if (packet == nullptr) {
                printf("Packet is empty (not present).\n");
                continue;     // Skip if nothing there.
            }

            printf("Packet of type %d -> %d events, %d capacity.\n",
              packet->getEventType(), packet->getEventNumber(),
              packet->getEventCapacity());

            if (packet->getEventType() == POLARITY_EVENT) {
                std::shared_ptr<const libcaer::events::PolarityEventPacket> \
                  polarity = std::static_pointer_cast<
                  libcaer::events::PolarityEventPacket>(packet);

                // Get full timestamp and addresses of first event.
                const libcaer::events::PolarityEvent &firstEvent = \
                  (*polarity)[0];

                int32_t ts = firstEvent.getTimestamp();
                uint16_t x = firstEvent.getX();
                uint16_t y = firstEvent.getY();
                bool pol = firstEvent.getPolarity();

                printf("First polarity - ts: %d, x: %d, y: %d, pol: %d.\n",
                  ts, x, y, pol);
            }

            if (packet->getEventType() == FRAME_EVENT) {
                std::shared_ptr<const libcaer::events::FrameEventPacket> \
                  frame = std::static_pointer_cast<
                  libcaer::events::FrameEventPacket>(packet);

                // Get full timestamp, and sum all pixels of first frame event.
                const libcaer::events::FrameEvent &firstEvent = (*frame)[0];

                int32_t ts = firstEvent.getTimestamp();
                uint64_t sum = 0;

                for (int32_t y = 0; y < firstEvent.getLengthY(); y++) {
                    for (int32_t x = 0; x < firstEvent.getLengthX(); x++) {
                        sum += firstEvent.getPixel(x, y);
                    }
                }
                printf("First frame event - ts: %d, sum: %" PRIu64 ".\n",
                  ts, sum);
            }
        }
    }

    davisHandle.dataStop();

    // Close automatically done by destructor.

    printf("Shutdown successful.\n");

    return (EXIT_SUCCESS);
}
