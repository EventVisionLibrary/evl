// Copyright 2018 Event Vision Library.
#include <iostream>
#include <libcaercpp/libcaer.hpp>
#include <libcaercpp/devices/davis.hpp>
#include "types.hpp"
#include "initialize_davis.hpp"

Davis initializeDavis(void) {
    // Open a DAVIS (ID of 1)
    Davis davisHandle = Davis(1);

    // Device information
    struct caer_davis_info davis_info = davisHandle.infoGet();
    printf("%s --- ID: %d, Master: %d, DVS X: %d, DVS Y: %d, Logic: %d.\n",
        davis_info.deviceString, davis_info.deviceID,
        davis_info.deviceIsMaster, davis_info.dvsSizeX,
        davis_info.dvsSizeY, davis_info.logicVersion);

    // Send the default configuration before using the device.
    davisHandle.sendDefaultConfig();

    struct caer_bias_coarsefine coarseFineBias;
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
    return davisHandle;
}
