// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
  * Copyright 2012 ASROB (http://asrob.uc3m.es)
  * This file is part of the Robot Devastation PC version
  * Author: Santiago Morante
  * Co-Author: Juan G. Victores
  */

#ifndef __EXECUTION_THREAD_HPP__
#define __EXECUTION_THREAD_HPP__

#include <cstring>
#include <iostream>

#include<opencv2/imgproc/imgproc.hpp>
//#include <cv.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <yarp/os/all.h>
#include <yarp/dev/all.h>

#include "GameControl.hpp"
#include "ScreenDraw.hpp"
#include "EnemyDetector.hpp"

#define DEFAULT_ROBOT "NONE"
#define DEFAULT_CABIN "ecro"
#define DEFAULT_MS 20.0

using namespace std;
using namespace cv;

namespace rdclient {

/**
 * @brief The fast periodic thread, e.g. controls video stream reception and screen refresh. Launched by \ref RdClient.
 */
class ExecutionThread : public yarp::os::RateThread {
  protected:
    bool dev;
    // #ifdef FORCE_DEV
    VideoCapture cap;
    // #else  // FORCE_DEV
    yarp::dev::PolyDriver rgbDev;
    yarp::dev::IFrameGrabberImage *iRgb;
    yarp::sig::ImageOf<yarp::sig::PixelRgb> imgRgb;
    // #endif  // FORCE_DEV
    Mat videoFrame, maskPic, picture;
    yarp::dev::PolyDriver robotDev;
    yarp::dev::IPositionControl *iPos;
    ScreenDraw screenDraw;
    GameControl gameControl;
    EnemyDetector enemyDetector;
    CascadeClassifier faceCascade;
    char key;

  public:
    /** 
     * Constructor.
     */
    ExecutionThread() : RateThread(DEFAULT_MS) {}  // In ms

    /**
     * Init method, pass the ResourceFinder class reference as it contains the configuration parameter dictionary.
     */
    bool init(yarp::os::ResourceFinder &rf);

    /**
     * Periodical loop function. This is the thread itself.
     */
    void run();
};

} //rdclient

#endif  // __EXECUTION_THREAD_HPP__

