// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdOutputHighgui.hpp"

rdlib::RdOutputHighgui::RdOutputHighgui()
{
    std::cout << "[info] RdOutputHighgui::RdOutputHighgui()"<< std::endl;

    //-- just for testing
    videoFrame = cv::imread("../share/rdClient/graphics/cabin_ecro.png");
    if(videoFrame.empty()) {
        std::cerr << "[warning] Image not found." << std::endl;
    }

}

bool rdlib::RdOutputHighgui::start()
{
    std::cout << "[info] RdOutputHighgui started." << std::endl;

    int width,height,step;
    rdCameraBasePtr->getDimensions(width,height,step);
    std::cout << "[info] RdOutputHighgui got camera [w:" << width << ", h:" << height << ", s:" << step  << "]" << std::endl;

    cv::namedWindow("Robot Devastation", CV_WINDOW_NORMAL);
   // cv::setWindowProperty("Robot Devastation", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    RdOutputBase::start();
}

char *rdlib::RdOutputHighgui::getInputKey()
{
    return inputKey;
}

pthread_mutex_t *rdlib::RdOutputHighgui::getInputKeyMutex()
{
    return inputKeyMutex;
}

bool rdlib::RdOutputHighgui::output(int pipelineIndex)
{
    //-- Display things:
    //std::cout << "[info] RdOutputHighgui alive..." << std::endl;

    cv::Mat image(cv::Size(width, height), CV_8UC3, rdCameraBasePtr->getBufferPtr(pipelineIndex), step);  // cv::Mat::AUTO_STEP ???
    printEnemies( image, image, pipelineIndex );
    cv::imshow("Robot Devastation", image);  // no cv:: needed.
    image.release();

    //std::cout << "[info] Displayed frame # " << pipelineIndex << "." << std::endl;

}


