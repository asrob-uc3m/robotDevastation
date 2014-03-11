// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdOutputHighgui.hpp"
#include <stdio.h>

rdlib::RdOutputHighgui::RdOutputHighgui()
{
    std::cout << "[info] RdOutputHighgui::RdOutputHighgui()"<< std::endl;

    //-- Create key & mutex
    inputKey = new char;
    *inputKey = -1;

    inputKeyMutex = new pthread_mutex_t;
    pthread_mutex_init( inputKeyMutex, NULL);

}

bool rdlib::RdOutputHighgui::start()
{
    std::cout << "[info] RdOutputHighgui started." << std::endl;

    cv::namedWindow("Robot Devastation", CV_WINDOW_NORMAL);
   // cv::setWindowProperty("Robot Devastation", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    RdOutputBase::start();
}

bool rdlib::RdOutputHighgui::quit()
{
    delete inputKey;
    delete inputKeyMutex;
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
    int width,height,step;
    rdCameraBasePtr->getDimensions(width,height,step);
    //std::cout << "[info] RdOutputHighgui got camera [w:" << width << ", h:" << height << ", s:" << step  << "]" << std::endl;

    cv::Mat image(cv::Size(width, height), CV_8UC3, rdCameraBasePtr->getBufferPtr(pipelineIndex), step);  // cv::Mat::AUTO_STEP ???
    printEnemies( image, image, pipelineIndex );
    cv::imshow("Robot Devastation", image);  // no cv:: needed.

    //std::cout << "[info] Displayed frame # " << pipelineIndex << "." << std::endl;
    pthread_mutex_lock(inputKeyMutex);
    *inputKey = cv::waitKey( 1 );
    pthread_mutex_unlock(inputKeyMutex);

    image.release();
}

bool rdlib::RdOutputHighgui::printEnemies(cv::Mat &src, cv::Mat &dst, int pipelineIndex)
{
    //-- If output is empty, clone input image
    if (dst.empty())
        dst = src.clone();

    //-- Get the enemies from the manager
    std::vector< std::pair< int, int> > enemyPos;
    std::vector< double > enemySize;
    rdManagerBasePtr->getEnemies( pipelineIndex, enemyPos, enemySize);

    //-- Draw the enemy markers
    for ( int i = 0; i < enemyPos.size() ; i++)
    {
        cv::rectangle( dst, cv::Point2d( enemyPos[i].first, enemyPos[i].second)
                       , cv::Point2d( enemyPos[i].first + enemySize[i]*2, enemyPos[i].second + enemySize[i]*2)
                       , cv::Scalar( 0, 0, 255));
    }
}


