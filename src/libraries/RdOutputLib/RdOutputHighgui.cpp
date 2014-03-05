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
    std::cout << "[info] RdOutputHighgui thread." << std::endl;
    while (!rdManagerBasePtr)
    {
        std::cout << "[info] RdOutputHighgui waiting for manager..." << std::endl;
        usleep(100000);
    }
    do
    {
        rdCameraBasePtr = rdManagerBasePtr->getRdCameraBasePtr();
        std::cout << "[info] RdOutputHighgui waiting for camera..." << std::endl;
        usleep(100000);
    }
    while ( ! (rdCameraBasePtr) );

    std::cout << "[info] RdOutputHighgui started." << std::endl;

    int width,height,step;
    rdCameraBasePtr->getDimensions(width,height,step);
    std::cout << "[info] RdOutputHighgui got camera [w:" << width << ", h:" << height << ", s:" << step  << "]" << std::endl;

    cv::namedWindow("Robot Devastation", CV_WINDOW_NORMAL);
    cv::setWindowProperty("Robot Devastation", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    while(1) {
        std::cout << "[info] RdOutputHighgui alive..." << std::endl;
        //char * bufferPtr = rdCameraBasePtr->getBufferPtr();
        //char bufferPtr[width* height];
        //bufferPtr = rdCameraBasePtr->getBufferPtr();
        cv::Mat image(cv::Size(width, height), CV_8UC3, rdCameraBasePtr->getBufferPtr(), step);  // cv::Mat::AUTO_STEP ???
        cv::imshow("Robot Devastation", image);  // no cv:: needed.
        cv::waitKey( 1 );
        image.release();
    }
}


