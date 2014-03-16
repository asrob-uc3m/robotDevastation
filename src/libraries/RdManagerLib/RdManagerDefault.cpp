// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdManagerDefault.hpp"

rdlib::RdManagerDefault::RdManagerDefault()
{
    this->managerStatus = MANAGER_STATUS_OK;

    //-- Add functions (currently only hardcoded)
    functionMap["toggleHeadTrack"] = (void *) &toggleHeadTrackWrapper;

    //-- Enemy init:
    for( int i = 0; i < PIPELINE_SIZE; i++)
    {
        enemyPos.push_back( std::vector< std::pair<int, int> >() );
        enemySize.push_back( std::vector< double> () );
    }

}

bool rdlib::RdManagerDefault::shoot() {
    if (!rdRobotBasePtr) return false;
    //
    return rdRobotBasePtr->shoot();
}

bool rdlib::RdManagerDefault::processImage()
{
    if (managerStatus == MANAGER_STATUS_OK)
    {
        do
        {
            for (int i = 0; i < PIPELINE_SIZE; i++)
            {
                //-- Lock the semaphore
                sem_wait( processSemaphores+i);

                //-- Right now, it doesn't do much
                std::cout << "[info] Processed frame # " << i << "." << std::endl;
                if ( managerStatus == MANAGER_STATUS_HEAD_TRACK )
                {
                    std::cout << "[info] Tracking head." << std::endl;
                    trackHead(i);
                }

                //-- Unlock the corresponding process semaphore
                sem_post(displaySemaphores+i);
            }
        } while(managerStatus != MANAGER_STATUS_STOPPED);
    }
    RD_SUCCESS("Exited manager main thread!");

}

bool rdlib::RdManagerDefault::toggleHeadTrackWrapper(void *This)
{
       (( rdlib::RdManagerDefault *) This)->toggleHeadTrack();
}

bool rdlib::RdManagerDefault::toggleHeadTrack()
{
    if ( managerStatus != MANAGER_STATUS_HEAD_TRACK )
        managerStatus = MANAGER_STATUS_HEAD_TRACK;
    else
        managerStatus = MANAGER_STATUS_OK;
}

bool rdlib::RdManagerDefault::trackHead( int index )
{
    //-- Create a haars cascade classifier
    cv::CascadeClassifier faceDetector;

    bool ok = faceDetector.load( "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml" );
    if (!ok)
        std::cerr << "[Debug] Cascade Classifier could not find template." << std::endl;

    //-- Get image
    int width,height,step;
    rdCameraBasePtr->getDimensions(width,height,step);
    cv::Mat image(cv::Size(width, height), CV_8UC3, rdCameraBasePtr->getBufferPtr(index), step);

    //-- Convert to grey and resize to find faces faster
    cv::Mat frameGrey, frameGreySmall;
    cv::cvtColor( image, frameGrey, CV_BGR2GRAY );
    cv::resize( frameGrey, frameGreySmall, cv::Size(0, 0), 0.5, 0.5 );

    std::vector< cv::Rect > detectedFaces;

    faceDetector.detectMultiScale( frameGreySmall, detectedFaces, 1.1, 2,
                                   0|CV_HAAR_SCALE_IMAGE, cv::Size( 30, 30) );

    //-- Resize the rectangles to get the original dimensions
    for (int i = 0; i < detectedFaces.size(); i++)
    {
        cv::Rect resizedRect = cv::Rect( detectedFaces[i].x * 2,
                                         detectedFaces[i].y * 2,
                                         detectedFaces[i].width * 2,
                                         detectedFaces[i].height * 2 );
        detectedFaces[i] = resizedRect;
    }

    std::vector< std::pair<int, int> > enemiesFound;
    std::vector< double > enemiesFoundSize;
    for ( int i = 0; i < detectedFaces.size(); i++)
    {
        enemiesFound.push_back( std::pair< int, int>( detectedFaces[i].x, detectedFaces[i].y ));
        enemiesFoundSize.push_back( sqrt( pow( detectedFaces[i].width / 2.0 , 2)
                                        + pow( detectedFaces[i].height / 2.0 , 2)  ) );
    }

    enemyPos[index] = enemiesFound;
    enemySize[index] = enemiesFoundSize;

     //-- Here I should call the functions that control the robot camera
     //-- towards the face
}

