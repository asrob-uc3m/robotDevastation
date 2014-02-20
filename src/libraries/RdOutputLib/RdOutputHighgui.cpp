// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdOutputHighgui.hpp"

namespace rdlib{

/************************************************************************/

RdOutputHighgui::RdOutputHighgui() {

    cv::namedWindow("Robot Devastation", CV_WINDOW_NORMAL);
    cv::setWindowProperty("Robot Devastation", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    //cv::imshow("Robot Devastation", videoFrame);  // cv:: not needed here

}

/************************************************************************/

RdOutputHighgui::~RdOutputHighgui() {
    std::cout << "[info] RdOutputHighgui::~RdOutputHighgui()"<< std::endl;
}

/************************************************************************/

bool RdOutputHighgui::stop() {
    std::cout << "[info] RdOutputHighgui stop()" << std::endl;
}

/************************************************************************/

} //rdlib

