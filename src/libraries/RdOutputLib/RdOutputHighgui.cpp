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

bool rdlib::RdOutputHighgui::showWeaponSight(cv::Mat &src, cv::Mat &dst, int pipelineIndex, rdlib::RdWeaponBase *currentWeapon)
{
    //-- If output is empty, clone input image
    if (dst.empty())
        dst = src.clone();

    cv::circle(dst, cv::Point(dst.cols/2, dst.rows/2), 5, cv::Scalar(0, 255, 0), 2);
    cv::circle(dst, cv::Point(dst.cols/2, dst.rows/2), 1, cv::Scalar(0, 255, 0), 1);
}

bool rdlib::RdOutputHighgui::showBullets(cv::Mat &src, cv::Mat &dst, int pipelineIndex, rdlib::RdWeaponBase *currentWeapon)
{
    //-- If output is empty, clone input image
    if (dst.empty())
        dst = src.clone();

    if (currentWeapon->getShootCurrentDelay() != 0)
    {
        cv::Point initPoint(dst.cols*0.75, dst.rows);
        int vx = (dst.cols/2-dst.cols*0.75)/currentWeapon->getShootDelay();
        int vy = (dst.rows/2-dst.rows)/currentWeapon->getShootDelay();

        int t = currentWeapon->getShootDelay()-currentWeapon->getShootCurrentDelay();

        int x = dst.cols*0.75+vx*t;
        int y = dst.rows+vy*t;

        RD_DEBUG("Shoot end at (%i, %i)\n", x, y);
        cv::Point endPoint(x, y);
        cv::line(dst, initPoint, endPoint, cv::Scalar(0, 0, 255), 3 );
        //cv::circle(dst, endPoint, 3, cv::Scalar(0, 0, 255), 2);
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
    //-- Display things:
    //std::cout << "[info] RdOutputHighgui alive..." << std::endl;
    int width,height,step;
    rdCameraBasePtr->getDimensions(width,height,step);
    //std::cout << "[info] RdOutputHighgui got camera [w:" << width << ", h:" << height << ", s:" << step  << "]" << std::endl;

    cv::Mat image(cv::Size(width, height), CV_8UC3, rdCameraBasePtr->getBufferPtr(pipelineIndex), step);  // cv::Mat::AUTO_STEP ???
    printEnemies( image, image, pipelineIndex );
    RdWeaponBase* currentWeapon = rdManagerBasePtr->getCurrentWeapon();
    showCurrentWeaponInfo(image, image, pipelineIndex, currentWeapon);
    showWeaponSight(image, image, pipelineIndex, currentWeapon);
    showBullets(image, image, pipelineIndex, currentWeapon);
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
    std::vector<RdEnemy>* enemies = NULL;
    pthread_mutex_t* mutexOfVectorOfRdEnemy = NULL;
    rdManagerBasePtr->getEnemies( &enemies, &mutexOfVectorOfRdEnemy );

    //-- Draw the enemy markers
    pthread_mutex_lock(mutexOfVectorOfRdEnemy);
    for ( int i = 0; i < enemies->size() ; i++)
    {
        RdVector2d pos = enemies->at(i).getPos();
        int width = enemies->at(i).getWidth();
        int height = enemies->at(i).getHeight();

        cv::Point2d lowerright = cv::Point2d( pos.x - width/2, pos.y - height / 2);
        cv::Point2d upperleft = cv::Point2d( pos.x + width/2, pos.y + height / 2);

        cv::rectangle( dst, upperleft, lowerright, cv::Scalar( 0, 0, 255));

        //-- Enemy health bar
        int health = enemies->at(i).getCurrentHealth();
        int max_health = enemies->at(i).getMaxHealth();

        cv::Point2d health_bar_upperleft( upperleft.x, upperleft.y-5);
        cv::Point2d health_bar_lowerright( lowerright.x, upperleft.y-15);

        int health_bar_level_lowerright_x = health_bar_upperleft.x +
                (health_bar_lowerright.x-health_bar_upperleft.x)*(int)(health/(double)max_health);
        cv::Point2d health_bar_level_lowerright( health_bar_level_lowerright_x,
                                                 health_bar_lowerright.y);

        cv::rectangle( dst, health_bar_upperleft, health_bar_lowerright, cv::Scalar( 0, 0, 255));
        cv::rectangle( dst, health_bar_upperleft, health_bar_level_lowerright, cv::Scalar( 0, 0, 255), CV_FILLED);


    }
    pthread_mutex_unlock(mutexOfVectorOfRdEnemy);
}


