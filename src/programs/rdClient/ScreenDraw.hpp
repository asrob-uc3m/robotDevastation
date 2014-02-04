// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
  * Copyright 2012 ASROB (http://asrob.uc3m.es)
  * This file is part of Robot Devastation PC version
  * Author: Santiago Morante
  */

#ifndef __SCREEN_DRAW_HPP__
#define __SCREEN_DRAW_HPP__

#include <iostream>
#include <cstring>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;

namespace rdclient{

/**
 * @brief Used to paint screen between refreshes.
 */
class ScreenDraw {

    public:
        Mat createChromaMask(const Mat& picture);
        bool checkChromaPixel (const Mat& videoFrame, const int i, const int j);
        Rect indicateDirection(const int& direction);
        Rect indicateTilt(const int& up_or_down);
        void shootLaserGuns(vector<Point>& references);

};

} //rdclient

#endif  // __SCREEN_DRAW_HPP__

