// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
  * Copyright 2012 ASROB (http://asrob.uc3m.es)
  * This file is part of Robot Devastation PC version
  * Author: Santiago Morante
  */

#ifndef __ENEMY_DETECTOR_HPP__
#define __ENEMY_DETECTOR_HPP__

#include <iostream>
#include <cstring>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"

#define DEFAULT_FACE_CASCADE_NAME "haarcascade_frontalface_alt.xml"
#define YELLOW Scalar(0,255,255)
#define RED Scalar(0,0,255)
#define ORANGE Scalar(0,69,255)

using namespace std;
using namespace cv;

namespace rdclient{

/**
 * @brief Uses OpenCV haar cascades to detect faces.
 */
class EnemyDetector{
  public:

    void humansDetector(Mat& frame, CascadeClassifier faceCascade);

};

} //rdclient

#endif  // __ENEMY_DETECTOR_HPP__

