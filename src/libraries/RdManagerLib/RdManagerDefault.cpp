// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdManagerDefault.hpp"

rdlib::RdManagerDefault::RdManagerDefault()
{
    std::cout << "[info] RdManagerDefault constructor." << std::endl;
    this->managerStatus = 0;

    //-- Add functions (currently only hardcoded)
    functionMap["toggleHeadTrack"] = (void *) &toggleHeadTrackWrapper;

    //-- Enemy init:
    for( int i = 0; i < 3; i++)
    {
        enemyPos.push_back( std::vector< std::pair<int, int> >() );
        enemySize.push_back( std::vector< double> () );
    }

}

bool rdlib::RdManagerDefault::start()
{
    //-- Start the capture thread
    pthread_create( &processImage_thread, NULL, processImageThread, (void *) this );
}

bool rdlib::RdManagerDefault::shoot() {
    if (!rdRobotBasePtr) return false;
    //
    return rdRobotBasePtr->shoot();
}

void * rdlib::RdManagerDefault::processImageThread(void *This)
{
    (( rdlib::RdManagerDefault *) This)->processImage();
}

bool rdlib::RdManagerDefault::processImage()
{
    while( managerStatus != -1 )
    {
        for (int i = 0; i < 3; i++)
        {
            //-- Lock the semaphore
            sem_wait( processSemaphores+i);

            //-- Right now, it doesn't do much
            //std::cout << "[info] Processed frame # " << i << "." << std::endl;
            if ( managerStatus == 1 )
            {
                std::cout << "[info] Tracking head." << std::endl;
                trackHead(i);
            }

            //-- Unlock the corresponding process semaphore
            sem_post( displaySemaphores+i);
        }
    }
}

bool rdlib::RdManagerDefault::toggleHeadTrackWrapper(void *This)
{
       (( rdlib::RdManagerDefault *) This)->toggleHeadTrack();
}

bool rdlib::RdManagerDefault::toggleHeadTrack()
{
    if ( managerStatus != 1 )
        managerStatus = 1;
    else
        managerStatus = 0;
}

bool rdlib::RdManagerDefault::trackHead( int index )
{
    //-- Create a haars cascade classifier
    cv::CascadeClassifier faceDetector;

    bool ok = faceDetector.load( "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml" );
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

     //-- Here I should call the functions that controls the robot camera
     //-- towards the face
}

bool rdlib::RdManagerDefault::quit()
{
    std::cout << "[info] RdManagerDefault quit." << std::endl;
    this->managerStatus = -1;
    pthread_join( processImage_thread, NULL );
    return true;
}

void rdlib::RdManagerDefault::setProcessSemaphores(sem_t *processSemaphores)
{
    this->processSemaphores = processSemaphores;
}

void rdlib::RdManagerDefault::setDisplaySemaphores(sem_t *displaySemaphores)
{
    this->displaySemaphores = displaySemaphores;
}

