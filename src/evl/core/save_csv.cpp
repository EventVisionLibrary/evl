// Copyright 2018 Event Vision Library.
#include <iostream>
#include <tuple>
#include <fstream>
#include "unistd.h"
#include "types.hpp"
#include "common.hpp"
#include "read_buffer.hpp"
#include "save_csv.hpp"


void loop_saveData(EventBuffer *buffer, int lifetime) {
    usleep(100000);      // micro sec
    std::string filename = "test.txt";
    std::ofstream writing_file;
    writing_file.open(filename);

    while (1) {
        usleep(100000);      // micro sec. calling frequency
        std::cout << "[Thread2] DATA SAVING =============" << std::endl;

        std::vector<EventTuple> v = readBufferOnLifetime(buffer, lifetime);
        for (auto itr = v.rbegin(); itr != v.rend(); ++itr) {
            writing_file << ' ' << std::get<0>(*itr) << ' ' << \
            std::get<1>(*itr) << ' ' << std::get<2>(*itr) << ' ' << \
             std::get<3>(*itr) << std::endl;
        }
        std::cout << "[Thread2] DATA SAVE DONE ===========" << std::endl;
    }
}
