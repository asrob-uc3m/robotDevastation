#include "RdCameraWebcam.hpp"
#include <stdio.h>

rdlib::RdCameraWebcam::RdCameraWebcam(int cameraIndex)
{
    //-- Set 'constants'
    frameRate = 10;
    stopThread = false;

    //-- Start the camera
    webcam.open(cameraIndex);
    for (int i = 0; i < PIPELINE_SIZE; i++)
    {
        cv::Mat frame;
        webcam.read(frame);
        imageBuffers.push_back(frame.clone() );
    }
}

void rdlib::RdCameraWebcam::capture( int index)
{
    webcam.read(imageBuffers.at(index));
}

char *rdlib::RdCameraWebcam::getBufferPtr(int index)
{
    return (char *) imageBuffers.at(index).data;
}

bool rdlib::RdCameraWebcam::getDimensions(int &width, int &height, int &step)
{
    width = imageBuffers.at(0).cols;
    height = imageBuffers.at(0).rows;
    step = imageBuffers.at(0).step[0];

    return true;
}


