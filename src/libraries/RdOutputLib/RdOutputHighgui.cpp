// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdOutputHighgui.hpp"

namespace rdlib{

/************************************************************************/

RdOutputHighgui::RdOutputHighgui() {
    std::cout << "[info] RdOutputHighgui::RdOutputHighgui()"<< std::endl;

    // just for testing
    videoFrame = cv::imread("../share/rdClient/graphics/cabin_ecro.png");
    if(videoFrame.empty()) {
        std::cerr << "[warning] Image not found." << std::endl;
    }

    cv::namedWindow("Robot Devastation", CV_WINDOW_NORMAL);
    cv::setWindowProperty("Robot Devastation", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    cv::imshow("Robot Devastation", videoFrame);  // cv:: not needed here
    cv::waitKey(0.1);  // libhighgui only renders with this

}

/************************************************************************/

RdOutputHighgui::~RdOutputHighgui() {
    std::cout << "[info] RdOutputHighgui::~RdOutputHighgui()"<< std::endl;
}

/************************************************************************/

bool RdOutputHighgui::quit() {
    std::cout << "[info] RdOutputHighgui quit()" << std::endl;
}

/************************************************************************/

} //rdlib

