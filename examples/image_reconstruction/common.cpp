#include "common.h"
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// #include "cnpy.h"

void saveEvents(std::string filename, std::vector<Event> &events) {
    // create text file and go through all events
    std::ofstream file;
    file.open(filename);
    for (int i=0;i<events.size();i++) {
        file << events[i].t << " " << events[i].x << " " << events[i].y << " " << events[i].polarity << std::endl;
    }
    file.close();
}

void loadEvents(std::string filename, std::vector<Event>& events, bool skip_events, bool flip_ud) {
    std::ifstream ifs;
    ifs.open(filename.c_str(),std::ifstream::in);

    if(ifs.good()) {
        Event temp_event;
        double time;
        double first_timestamp=0;
        double last_timestamp=0;
        bool normalize_time=true;
        if (skip_events)
            for(int i=0;i<30000;i++) {// throw away the on-events
                ifs >> time >> temp_event.y >> temp_event.x >> temp_event.polarity;
            }
        while(!ifs.eof()) {
            ifs >> time;
            if(first_timestamp==0) {
                first_timestamp=time;
                if(time<1)
                    normalize_time = false;
            }
            time -= first_timestamp;
            ifs >> temp_event.x;
            ifs >> temp_event.y;
            if(flip_ud)
                temp_event.y = 127-temp_event.y;
            ifs >> temp_event.polarity;
            last_timestamp=time;
            temp_event.t = time*(normalize_time? TIME_CONSTANT:1);
            temp_event.polarity=temp_event.polarity>0?1:-1;
            events.push_back(temp_event);
        }
        ifs.close();
    }
}


void saveState(std::string filename, const cv::Mat *mat, bool as_png, bool as_npy) {
    // if(as_npy) {
    //     // save current image as npy
    //     cnpy::npy_save(filename + ".npy", mat, shape, 2);
    // }
    if(as_png) {
        // save current image as png
        cv::Mat img;
        (*mat).copyTo(img);
        cv::imwrite(filename + ".png", img);
    }
}

void loadState(std::string filename, cv::Mat *mat, float u_min) {
    // read png file
    if(filename.find(".png") != std::string::npos) {
        cv::Mat image = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
        image.convertTo(image, CV_32FC1, 1.0/255.0);
        image.copyTo(*mat);
    }
    double minVal, maxVal;
    cv::minMaxLoc(*mat, &minVal, &maxVal);
    *mat = *mat + u_min - minVal;
}
