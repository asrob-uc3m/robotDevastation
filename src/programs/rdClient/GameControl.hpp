// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
  * Copyright 2012 ASROB (http://asrob.uc3m.es)
  * This file is part of Robot Devastation PC version.
  * Author: Santiago Morante
  */

#ifndef __GAME_CONTROL_HPP__
#define __GAME_CONTROL_HPP__

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "ScreenDraw.hpp"
#include <yarp/dev/all.h>

#define YELLOW Scalar(0,255,255)
#define RED Scalar(0,0,255)
#define GREEN Scalar(0,255,0)
#define BLUE Scalar(255,0,0)
#define WHITE Scalar(255,255,255)

using namespace std;
using namespace cv;

namespace rdclient {

/**
 * @brief Controls key presses and send commands to actual robot.
 */
class GameControl {
    protected:

        /** Velocity controller pointer. */
        yarp::dev::IVelocityControl *iVel;

    public:
        static void onMouse(int event, int x, int y, int flags, void* param );
        void keyboardActions(Mat& image, const int key);
        void setGun(int key=49);
        void resetAmmo(int bullets=10){ammo=bullets;}
        void reduceAmmo(){ammo-=1;}
        void showAmmo(Mat& image);
        int gunThickness;
        Scalar gunColor;
        int ammo;
        
        /** Pass a velocity controller pointer to the class. */
        void setVelocityControl(yarp::dev::IVelocityControl* _iVel);
};

} //rdclient

#endif  // __GAME_CONTROL_HPP__

