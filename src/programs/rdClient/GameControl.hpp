// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
  * Copyright 2013 ASROB (http://asrob.uc3m.es)
  * This file is part of Robot Devastation PC version.
  * Authors: Santiago Morante, David Estévez Fernández
  */

#ifndef __GAME_CONTROL_HPP__
#define __GAME_CONTROL_HPP__

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "ScreenDraw.hpp"
#include <yarp/dev/all.h>

static const Scalar YELLOW = Scalar(0,255,255);
static const Scalar RED = Scalar(0,0,255);
static const Scalar GREEN = Scalar(0,255,0);
static const Scalar BLUE = Scalar(255,0,0);
static const Scalar WHITE = Scalar(255,255,255);

using namespace std;
using namespace cv;

namespace rdclient {

/**
 * @brief Controls key presses and send commands to actual robot.
 */
class GameControl {

    public:
    GameControl();
    static void onMouse(int event, int x, int y, int flags, void* param );
    void keyboardActions(Mat& image, const int key);
    void setGun(int key=49);
    void resetAmmo(int bullets=10){ammo=bullets;}
    void reduceAmmo(){ammo-=1;}
    void showAmmo(Mat& image);
    void showKills(Mat& image);
    void updateEnemies( std::vector< cv::Rect > * enemies );
    void checkKilled();

    /** Pass a velocity controller pointer to the class. */
    void setVelocityControl(yarp::dev::IVelocityControl* _iVel);

    protected:

        /** Velocity controller pointer. */
        yarp::dev::IVelocityControl *iVel;

        int gunThickness;
        Scalar gunColor;
        int ammo;
        int kills;
        std::vector< cv::Rect > * enemies;



};

} //rdclient

#endif  // __GAME_CONTROL_HPP__

