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

    //-- Init the semaphores
    captureSemaphores = new sem_t[PIPELINE_SIZE];
    for( int i = 0; i < PIPELINE_SIZE; i++)
        sem_init( captureSemaphores+i, 0, 1);

    processSemaphores = new sem_t[PIPELINE_SIZE];
    for( int i = 0; i < PIPELINE_SIZE; i++)
        sem_init( processSemaphores+i, 0, 0);

    displaySemaphores = new sem_t[PIPELINE_SIZE];
    for( int i = 0; i < PIPELINE_SIZE; i++)
        sem_init( displaySemaphores+i, 0, 0);

}


void rdlib::RdCameraWebcam::capture( int index)
{
    webcam.read(imageBuffers.at(index));
    //std::cout << "[info] Captured frame # " << index << "." << std::endl;
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


