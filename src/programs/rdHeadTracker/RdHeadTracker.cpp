// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdHeadTracker.hpp"

#include "RdInputKeyboard.hpp"
#include "RdRobotLaserTowerOfDeath.hpp"

typedef bool (*pfunc)( void *);

namespace rdheadtracker {

/************************************************************************/
RdHeadTracker::RdHeadTracker() { }

/************************************************************************/
bool RdHeadTracker::configure(yarp::os::ResourceFinder &rf) {

    watchdog = rf.check("watchdog",DEFAULT_WATCHDOG).asDouble();
    std::cout << "RdHeadTracker using watchdog [s]: " << watchdog << " (default: " << DEFAULT_WATCHDOG << ")." << std::endl;

    rdInputBasePtr = new rdlib::RdInputKeyboard();
    rdInputBasePtr->init();

    rdRobotBasePtr = new rdlib::RdRobotLaserTowerOfDeath();
//    bool (*myFunction) (void *);
//    myFunction = (bool (*)(void*))( rdRobotBasePtr->getFunctionByName("shoot") );
//    (*myFunction)( (void*) rdRobotBasePtr);

    //-- Create & open webcam:
    cv::VideoCapture cap;
    cap.open(1);

    if ( !cap.isOpened() )
    {
        std::cerr << "Device could not be opened." << std::endl;
        return false;
    }

    //-- Create haars cascade clasifier
    cv::CascadeClassifier faceDetector;
    faceDetector.load( "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml" );

    while(1)
    {
        //-- Read webcam image:
        cv::Mat frame, display;
        if ( !cap.read( frame) )
            return false;

        display = frame.clone();

        //-- Convert to grey and resize to find faces faster
        cv::Mat frameGrey, frameGreySmall;
        cv::cvtColor( frame, frameGrey, CV_BGR2GRAY );
        cv::resize( frameGrey, frameGreySmall, cv::Size(0, 0), 0.5, 0.5 );

        std::vector< cv::Rect > detectedFaces;

        faceDetector.detectMultiScale( frameGreySmall, detectedFaces, 1.1, 2,
                                       0|CV_HAAR_SCALE_IMAGE, cv::Size( 30, 30) );

        //-- Resize the rectangles to get the original dimensions
        for (int i = 0; i < detectedFaces.size(); i++)
        {
            cv::Rect resizedRect = cv::Rect( detectedFaces[i].x * 2,
                                             detectedFaces[i].y * 2,
                                             detectedFaces[i].width  * 2,
                                             detectedFaces[i].height * 2 );
            detectedFaces[i] = resizedRect;
        }

        if (detectedFaces.size() > 0 )
        {
            //-- Get the first face detected
            cv::Rect mainFace = detectedFaces[0];

            //-- Get coordinates of the center of the face:
            int x, y;
            x = mainFace.x;
            y = mainFace.y;

            //-- Get the coordinates of the center of the image:
            int centerX, centerY;
            centerX = frame.cols / 2;
            centerY = frame.rows / 2;

            //-- Move the camera according to those values:
            static const int threshold = 25;

            if (  x < centerX - threshold )
            {
                pfunc panIncrement = (pfunc) ( rdRobotBasePtr->getFunctionByName("panIncrement"));
                panIncrement( (void *) rdRobotBasePtr );
            }
            else if ( x > centerX + threshold  )
            {
                pfunc panDecrement = (pfunc) ( rdRobotBasePtr->getFunctionByName("panDecrement"));
                panDecrement( (void *) rdRobotBasePtr );
            }

            if ( y > centerY + threshold  )
            {
                pfunc tiltIncrement = (pfunc) ( rdRobotBasePtr->getFunctionByName("tiltIncrement"));
                tiltIncrement( (void *) rdRobotBasePtr );
            }
            else if ( y < centerY - threshold )
            {
                pfunc tiltDecrement = (pfunc) ( rdRobotBasePtr->getFunctionByName("tiltDecrement"));
                tiltDecrement( (void *) rdRobotBasePtr );
            }

            //-- Just for debugging purposes print faces on display
            for ( int i = 0; i < detectedFaces.size(); i++)
                cv::rectangle( display, detectedFaces[i], cv::Scalar( 0, 0, 255) );
        }

        cv::imshow( "HeadTracker", display);

        char key = cv::waitKey(10);
        if ( key == 27 || key == 'q' )
            return true;
    }

    return executionThread.start();
}

/************************************************************************/

bool RdHeadTracker::updateModule() {
    std::cout << "RdHeadTracker alive..." << std::endl;
    return true;
}

/************************************************************************/

double RdHeadTracker::getPeriod() {
    return watchdog;  // [s]
}

/************************************************************************/

bool RdHeadTracker::interruptModule() {  // Closing rutines.
    delete rdInputBasePtr;
    rdInputBasePtr = 0;
    return true;
}

/************************************************************************/

} //RdHeadTracker

