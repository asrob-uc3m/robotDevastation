#include "RdCameraWebcam.hpp"

rdlib::RdCameraWebcam::RdCameraWebcam(int index)
{
    //-- Set 'constants'
    frameRate = 10;
    stopThread = false;

    //-- Start the camera
    webcam.open(index);
    for (int i = 0; i < 3; i++)
    {
        cv::Mat frame;
        webcam.read(frame);
        imageBuffers.push_back(frame.clone() );
    }

    //-- Init the semaphores
    captureSemaphores = new sem_t[3];
    for( int i = 0; i < 3; i++)
        sem_init( captureSemaphores+i, 0, 1);

    processSemaphores = new sem_t[3];
    for( int i = 0; i < 3; i++)
        sem_init( processSemaphores+i, 0, 0);

    displaySemaphores = new sem_t[3];
    for( int i = 0; i < 3; i++)
        sem_init( displaySemaphores+i, 0, 0);

}

bool rdlib::RdCameraWebcam::start()
{
    //-- Start the capture thread
    pthread_create( &capture_thread, NULL, captureThread, (void *) this );
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


void *rdlib::RdCameraWebcam::captureThread(void *This)
{
    ( (rdlib::RdCameraWebcam *) This)->capture();
}


void rdlib::RdCameraWebcam::capture()
{
    while( !stopThread )
    {
        for (int i = 0; i < 3; i++)
        {
            //-- Lock the semaphore
            sem_wait( captureSemaphores+i);

            //-- Get a frame
            webcam.read(imageBuffers.at(i));
            //std::cout << "[info] Captured frame # " << i << "." << std::endl;

            //-- Unlock the corresponding process semaphore
            sem_post( processSemaphores+i);

            //-- Wait
            //usleep( frameRate * 1000);
        }
     }
}

bool rdlib::RdCameraWebcam::setStop(bool stop)
{
    this->stopThread = stop;
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

sem_t *rdlib::RdCameraWebcam::getCaptureSemaphores()
{
    return captureSemaphores;
}

sem_t *rdlib::RdCameraWebcam::getProcessSemaphores()
{
    return processSemaphores;
}

sem_t *rdlib::RdCameraWebcam::getDisplaySemaphores()
{
    return displaySemaphores;
}


