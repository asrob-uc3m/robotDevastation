#include "RdCameraWebcam.hpp"

rdlib::RdCameraWebcam::RdCameraWebcam(int index)
{
    //-- Set 'constants'
    frameRate = 10;
    stopThread = false;

    //-- Start the camera
    webcam.open(index);
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


bool rdlib::RdCameraWebcam::quit()
{
    std::cout << "[info] RdCameraWebcam quit()" << std::endl;
    stopThread = true;
    pthread_join( capture_thread, NULL);

    delete[] captureSemaphores;
    captureSemaphores = 0;
    delete[] processSemaphores; //-- Doing this here will cause (probably) a segmentation fault
    processSemaphores = 0;
    delete[] displaySemaphores;
    displaySemaphores = 0;
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


