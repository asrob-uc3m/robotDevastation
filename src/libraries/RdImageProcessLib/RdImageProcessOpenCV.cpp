// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdImageProcessOpenCV.hpp"

bool rdlib::RdImageProcessOpenCV::setup()
{
    if(!getenv("RD_ROOT"))
    {
        RD_ERROR("Required RD_ROOT environomental variable empty!\n");
        RD_ERROR("Include the line \"export RD_ROOT=/your/path/to/robotDevastation\" or similar in .bashrc or .profile\n");
        return false;
    }
    std::string faceDetectorFilename( getenv("RD_ROOT") );
    faceDetectorFilename += "/share/rdClient/xml/haarcascade_frontalface_alt.xml";
    if ( ! faceDetector.load( faceDetectorFilename.c_str() ) )
    {
        RD_WARNING("Cascade Classifier could not find template.\n");
    }
    else
    {
        RD_SUCCESS("Cascade Classifier found template.\n");
    }
    return true;
}

bool rdlib::RdImageProcessOpenCV::process(char * imgPtr, const int width, const int height, const int step,
                                          std::vector<std::pair<int, int> > &enemyPos,
                                          std::vector<double> &enemySize)
{
    //-- Put image in cv::Mat
    cv::Mat image(cv::Size(width, height), CV_8UC3, imgPtr, step);

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

    enemiesFound.clear();
    enemiesFoundSize.clear();
    for ( int i = 0; i < detectedFaces.size(); i++)
    {
        enemiesFound.push_back( std::pair< int, int>( detectedFaces[i].x, detectedFaces[i].y ));
        enemiesFoundSize.push_back( sqrt( pow( detectedFaces[i].width / 2.0 , 2)
                                        + pow( detectedFaces[i].height / 2.0 , 2)  ) );
    }

    enemyPos = enemiesFound;
    enemySize = enemiesFoundSize;

     //-- Here I should call the functions that control the robot camera
     //-- towards the face
    return true;
}

