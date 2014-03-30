// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdOutputHighgui.hpp"
#include <stdio.h>

rdlib::RdOutputHighgui::RdOutputHighgui()
{
    //-- Create key & mutex
    inputKey = new char;
    *inputKey = -1;

    inputKeyMutex = new pthread_mutex_t;
    pthread_mutex_init( inputKeyMutex, NULL);

}

bool rdlib::RdOutputHighgui::start()
{
    cv::namedWindow("Robot Devastation", CV_WINDOW_NORMAL);
   // cv::setWindowProperty("Robot Devastation", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    RdOutputBase::start();
}

bool rdlib::RdOutputHighgui::quit()
{
    delete inputKey;
    delete inputKeyMutex;
}

bool rdlib::RdOutputHighgui::showCurrentWeaponInfo(cv::Mat &src, cv::Mat &dst, int pipelineIndex, rdlib::RdWeaponBase *currentWeapon)
{
    //-- If output is empty, clone input image
    if (dst.empty())
        dst = src.clone();

    //-- Draw the ammo level
    float ammo_percentage = currentWeapon->getCurrentAmmo() / (float) currentWeapon->getMaxAmmo();
    cv::Point barUpperLeft( dst.cols*0.8, dst.rows*0.10 );
    cv::Point barLowerRight( dst.cols*0.98, dst.rows*0.15);
    cv::Point ammoLowerRight( dst.cols*0.8+(dst.cols*(0.98-0.8)*ammo_percentage), dst.rows*0.15);

    cv::rectangle(dst, barUpperLeft, barLowerRight, cv::Scalar(0, 255, 0));
    cv::rectangle(dst, barUpperLeft, ammoLowerRight, cv::Scalar(0, 255, 0), CV_FILLED);
    cv::putText(dst, "Ammo", cv::Point( dst.cols*0.7, dst.rows*0.15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255,0));


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
    RdWeaponBase* currentWeapon = rdManagerBasePtr->getCurrentWeapon();
    showCurrentWeaponInfo(image, image, pipelineIndex, currentWeapon);
    cv::imshow("Robot Devastation", image);

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


