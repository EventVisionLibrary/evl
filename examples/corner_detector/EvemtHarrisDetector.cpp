#include "EventHarrisDetector.h"

EventHarrisDetector::EventHarrisDetector(const char *fname, const double lifetime) {
    FILE *fp;
    int ret;
    double ts; double x; double y;
    double pol_raw; bool pol;


    //Reading all event data from csv and save it to que.
    fp = fopen(fname, "r");
    if (fp == NULL) {
        printf("%sThe file cannot be opened!\n", fname);
        return;
    }
    int count =0;

    while ((ret=fscanf(fp, "%lf %lf %lf %lf", &ts, &x, &y, &pol_raw)) != EOF) {
        //Note: If you load .csv file, you should use "%lf, %lf, %lf %lf"
        pol = static_cast<bool>(pol_raw);
        evl::EventTuple tup = std::make_tuple(ts, x, y, pol);
        que.push(tup);
        count++;
    }
    fclose(fp);

    return;
}

std::vector<cv::Point2f> EventHarrisDetector::detect(const std::queue<evl::EventTuple>* ev_slice){
    std::vector <cv::Point2f> corners;
    std::deque<std::pair<int, int>> pos_queue(NUM_EVENTS+1), neg_queue(NUM_EVENTS+1);
    cv::Mat binaryOn, binaryOff;
    cv::Mat dx, dy, dx2, dy2, dxy, dx2_, dy2_, dxy_;
    cv::Mat h;

    binaryOn = cv::Mat::zeros(H, W, CV_8UC1);
    binaryOff = cv::Mat::zeros(H, W, CV_8UC1);

    h = cv::getGaussianKernel(L, 1, CV_32F);
    h = h * h.t();

    while(!ev_slice->empty()) {
        //Create image from events.
        evl::EventTuple event_cur = ev_slice->front();
        ev_slice->pop();
        double ts = std::get<0>(event_cur);
        double x = std::get<1>(event_cur);
        double y = std::get<2>(event_cur);
        double pol = std::get<3>(event_cur);

        if (x <= L or x >= W - L or y <= L or y >= H - L)
            continue;

        if (pol == POSITIVE) {
            binaryOn.at<uchar>(y, x) = 1;
            pos_queue.push_back(std::make_pair(x, y));
            if (pos_queue.size() == (NUM_EVENTS + 1)) {
                std::pair<int, int> oldestPos = pos_queue.front();
                binaryOn.at<uchar>(oldestPos.second, oldestPos.first) = 0;
                pos_queue.pop_front();
            }

            cv::Rect ROI = cv::Rect(x - L, y - L, 2 * L + 1, 2 * L + 1);
            cv::Mat mat_roi = binaryOn(ROI);
            cv::Sobel(mat_roi, dx, CV_64F, 1, 0, 7, 0.01, cv::BORDER_CONSTANT);
            cv::Sobel(mat_roi, dy, CV_64F, 0, 1, 7, 0.01, cv::BORDER_CONSTANT);
            dx2 = dx.mul(dx);
            dy2 = dy.mul(dy);
            dxy = dx.mul(dy);
            cv::Mat h_ = cv::getGaussianKernel(2 * L + 1, 1);
            cv::Mat h = h_ * h_.t();

            dx2_ = dx2.mul(h);
            dy2_ = dy2.mul(h);
            dxy_ = dxy.mul(h);
            double a = cv::sum(dx2_)[0];
            double d = cv::sum(dy2_)[0];
            double b = cv::sum(dxy_)[0];
            double c = b;
            Eigen::Matrix2d M;
            M << a, b,
                    c, d;
            double score = M.determinant() - 0.04 * (M.trace() * M.trace());

            if (score > HARRIS_THRESHOLD)
                corners.push_back(cv::Point2f(x,y));

        }else{
            binaryOff.at<uchar>(y,x)=1;
            neg_queue.push_back(std::make_pair(x,y));
            if (neg_queue.size() == (NUM_EVENTS+1)) {
                std::pair<int, int> oldestPos = neg_queue.front();
                binaryOff.at<uchar>(oldestPos.second, oldestPos.first) = 0;
                neg_queue.pop_front();
            }

            cv::Rect ROI = cv::Rect(x - L, y - L, 2 * L + 1, 2 * L + 1);
            cv::Mat mat_roi = binaryOff(ROI);
            cv::Sobel(mat_roi, dx, CV_64F, 1, 0, 7, 0.01, cv::BORDER_CONSTANT);
            cv::Sobel(mat_roi, dy, CV_64F, 0, 1, 7, 0.01, cv::BORDER_CONSTANT);
            dx2 = dx.mul(dx);
            dy2 = dy.mul(dy);
            dxy = dx.mul(dy);
            cv::Mat h_ = cv::getGaussianKernel(2 * L + 1, 1);
            cv::Mat h = h_ * h_.t();

            dx2_ = dx2.mul(h);
            dy2_ = dy2.mul(h);
            dxy_ = dxy.mul(h);
            double a = cv::sum(dx2_)[0];
            double d = cv::sum(dy2_)[0];
            double b = cv::sum(dxy_)[0];
            double c = b;
            Eigen::Matrix2d M;
            M << a, b,
                    c, d;
            double score = M.determinant() - 0.04 * (M.trace() * M.trace());

            if (score > HARRIS_THRESHOLD)
                corners.push_back(cv::Point2f(x,y));
        }
    }

    return corners;
}

void EventHarrisDetector::visualize() {
    std::vector<evl::EventTuple> v;
    std::queue<evl::EventTuple> time_slice;
    double start_time;
    double current_time;
    cv::Mat img, harris_img;
    std::vector <cv::Point2f> corners;

    while(!que.empty()) {
        //Create image from events.
        start_time = std::get<0>(que.front());
        current_time = start_time;
        while (!que.empty() and current_time < start_time + lifetime) {
            v.push_back(que.front());
            time_slice.push(que.front());
            current_time = std::get<0>(que.front());
            que.pop();
        }
        img = evl::events_to_mat(v, true);

        //Harris corner detection
        harris_img = img.clone();
        corners = EventHarrisDetector(&time_slice);
        std::vector<cv::Point2f>::iterator it_corner = corners.begin();
        for (; it_corner != corners.end(); ++it_corner) {
            circle(harris_img, cv::Point(it_corner->x, it_corner->y), 2, cv::Scalar(0, 255, 0), -1);
        }

        //free memory
        std::vector<evl::EventTuple>().swap(v);

        //Visualization
        cv::imshow("DVS", img);
        cv::imshow("Harris", harris_img);
        cv::waitKey(3); //Note: This "3" is a magic number to show the video stream like real-time.
    }

    return;
}

