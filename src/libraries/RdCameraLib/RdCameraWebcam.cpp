#include "RdCameraWebcam.hpp"

rdlib::RdCameraWebcam::RdCameraWebcam(int index)
{
    //-- Set 'constants'
    frameRate = 10;
    stop = false;

    //-- Start the camera
    webcam.open(index);
    webcam.read(imageBuffer);

    //-- Init the mutex
    thread_mutex_init( &imageBufferMutex, NULL );

    //-- Start the capture thread
    pthread_create( capture_thread, NULL, capturethread, (void *) this );
}


bool rdlib::RdCameraWebcam::quit()
{
    pthread_join( capture_thread, NULL);
}


void *rdlib::RdCameraWebcam::captureThread(void *This)
{
    ( (rdlib::RdCameraWebcam *) This)->capture();
}


void rdlib::RdCameraWebcam::capture()
{
    while( !stop )
    {
        //-- Lock the mutex
        pthread_mutex_lock(&imageBufferMutex);

        //-- Get a frame
        webcam.read(imageBuffer);

        //-- Unlock the mutex
        pthread_mutex_unlock(&imageBufferMutex);

        //-- Wait
        usleep( frameRate * 1000);
    }
}

bool rdlib::RdCameraWebcam::setStop(bool stop)
{
    this->stop() = stop;
}

char *rdlib::RdCameraWebcam::getBufferPtr()
{
    return imageBuffer.data;
}

bool rdlib::RdCameraWebcam::getDimensions(int &width, int &height, int &step)
{
    width = imageBuffer.cols;
    height = imageBuffer.rows;
    step = imageBuffer.step[0];

    return true;
}
