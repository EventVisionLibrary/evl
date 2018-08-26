#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// #include <boost/program_options.hpp>
// #include <boost/filesystem.hpp>

#include "cnpy.h"
#include "denoise.h"
#include "common.h"
#include "event.h"

// namespace po = boost::program_options;

constexpr unsigned int str2int(const char* str, int h = 0) {
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

enum {
    NONE, OCC, TIME
};

int main(int argc, char *argv[]) {
    if (argc == 0){return 1;}
    std::string eventfile = argv[1];
    std::string initialimage = "";
    int width = 240;
    int height = 180;
    double lambda = 180.0;
    double lambda_t = 2.0;
    double u_min = 1.0;
    double u_max = 2.0;
    double C1 = 1.15;
    double C2 = 1.20;
    int iterations = 50;
    int events_per_image = 1000;
    std::string outputfolder = "./";
    METHOD method = TV_KLD;
            // method = TV_L1;
            // method = TV_L2;
            // method = TV_LogL2;
    // parameter parsing using boost program_options
    // po::options_description desc("Allowed options");
    // desc.add_options()
    //     ("help", "produce help message")
    //     ("event-file,f", po::value<std::string>(), "File that contains events")
    //     ("initial-image,n", po::value<std::string>()->default_value(""), "File that contains u_0")
    //     ("events-per-image,e", po::value<int>()->default_value(1000),"Events per reconstructed frame")
    //     ("lambda-data,d", po::value<double>()->default_value(180.0),"Lambda for Data Term")
    //     ("lambda-time,t", po::value<double>()->default_value(2.0),"Lambda for Time Manifold")
    //     ("iterations,i", po::value<int>()->default_value(50),"Number of iterations per image")
    //     ("u-min", po::value<double>()->default_value(1.0),"Minimum value of reconstruction")
    //     ("u-max", po::value<double>()->default_value(2.0),"Maximum value of reconstruction")
    //     ("c1", po::value<double>()->default_value(1.15),"Positive threshold")
    //     ("c2", po::value<double>()->default_value(1.20),"Negative threshold")
    //     ("method,m", po::value<std::string>()->default_value("TVEntropy"), "Method. Possible options: TVL1, TVL2, TVLog2, TVEntropy")
    //     ("width,w", po::value<int>()->default_value(240),"Width")
    //     ("height,h", po::value<int>()->default_value(180),"Height")
    //     ("output-folder,o", po::value<std::string>()->default_value("./"),"Folder where all the file will be written")
    // ;
    // po::positional_options_description p;
    // p.add("event-file", -1);
    // po::variables_map vm;
    // po::store(po::command_line_parser(argc, argv).
    //           options(desc).positional(p).run(), vm);
    // po::notify(vm);
    //
    // if (!vm.count("event-file")) {
    //     std::cout << desc  << "\n";
    //     return 1;
    // }
    // int width = vm["width"].as<int>();
    // int height = vm["height"].as<int>();
    // double lambda = vm["lambda-data"].as<double>();
    // double lambda_t = vm["lambda-time"].as<double>();
    // double u_min = vm["u-min"].as<double>();
    // double u_max = vm["u-max"].as<double>();
    // double C1 = vm["c1"].as<double>();
    // double C2 = vm["c2"].as<double>();
    // int iterations = vm["iterations"].as<int>();
    // int events_per_image = vm["events-per-image"].as<int>();
    // std::string outputfolder = vm["output-folder"].as<std::string>();
    // METHOD method;
    // switch(str2int(vm["method"].as<std::string>().c_str())) {
    //     case str2int("TVEntropy"):
    //         method = TV_KLD;
    //         break;
    //     case str2int("TVL1"):
    //         method = TV_L1;
    //         break;
    //     case str2int("TVL2"):
    //         method = TV_L2;
    //         break;
    //     case str2int("TVLog2"):
    //         method = TV_LogL2;
    //         break;
    //     default:
    //         std::cout << desc  << "\n";
    //         return 1;
    // }


    // read events from file
    std::vector <Event> events;
    // loadEvents(vm["event-file"].as<std::string>(), events);
    loadEvents(eventfile, events, false, false);

    // prepare memory
    cv::Mat input = cv::Mat::ones(width, height, CV_32FC1) * (u_min + u_max) / 2.f;    // or [H, W]?  or eigen?
    cv::Mat manifold = cv::Mat::zeros(width, height, CV_32FC1);
    cv::Mat output = cv::Mat::ones(width, height, CV_32FC1) * (u_min + u_max) / 2.f;
    cv::Mat occurences = cv::Mat::zeros(width, height, CV_32SC1);

    ir::initDenoise(&input, &manifold);    // TODO(shiba) rewrite

    cv::Mat events_host = cv::Mat::zeros(events_per_image, 1, CV_32FC4);

    // see if we have an initial image
    // std::string initial_image_filename = vm["initial-image"].as<std::string>();
    std::string initial_image_filename = initialimage;
    if (initial_image_filename.compare("") != 0) {
        loadState(initial_image_filename, &input, u_min);
        input.copyTo(output);
    }

    // prepare output folder
    // boost::filesystem::create_directories(outputfolder);

    // iterate over the "images"
    int event_id = 0;
    std::ofstream timestampfile(outputfolder + "/frametimestamps.txt");
    while(event_id < events.size() - events_per_image) {
        // always process packets
        for (int i = 0; i < events_per_image; i++) {
            // cv::Vec4b *p = events_host.ptr<cv::Vec4b>(i, 1);
            events_host.at<cv::Vec4b>(i, 1)[0] = (float)(events[i+event_id].x);
            events_host.at<cv::Vec4b>(i, 1)[1] = (float)(events[i+event_id].y);
            events_host.at<cv::Vec4b>(i, 1)[2] = (float)(events[i+event_id].polarity);
            events_host.at<cv::Vec4b>(i, 1)[3] = (float)(events[i+event_id].t);
        }
        event_id += events_per_image;

        // TODO(shiba) rewrite
        ir::solveTVIncrementalManifold(&output,&input,&manifold,lambda,lambda_t,iterations,u_min,u_max,method);

        // save to file
        std::stringstream outfilename;
        outfilename << outputfolder << "/image" << std::setfill('0') << std::setw(6) << event_id / events_per_image;
        saveState(outfilename.str(),&output,true,false);
        timestampfile << events[event_id].t << std::endl;
    }
    timestampfile.close();

    return EXIT_SUCCESS;
}
