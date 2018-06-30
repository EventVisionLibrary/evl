// Copyright 2018 Event Vision Library.
#include <iostream>
#include <tuple>
#include <fstream>

#include "unistd.h"

#include "types.hpp"
#include "common.hpp"

#include "buffer_csv.hpp"

void bufferData(EventBuffer *buffer, char* fname) {
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

    while ((ret = fscanf(fp, "%lf,%lf,%lf,%lf", &ts, &x, &y, &pol_raw)) != EOF) {
    // while ((ret = fscanf(fp, "%u,%hu,%hu,%d", &ts, &x, &y, &pol_raw)) != EOF) {
        pol = static_cast<bool>(pol_raw);
        usleep(100);
        mtx.lock();
        EventTuple tup = std::make_tuple(ts, x, y, pol);
        (*buffer).push_front(tup);
        mtx.unlock();
    }
    fclose(fp);
    std::cout << "[Thread1] Buffering Loop Finished!! " << std::endl;
}
