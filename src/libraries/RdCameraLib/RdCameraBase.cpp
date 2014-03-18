#include "RdCameraBase.hpp"

bool rdlib::RdCameraBase::setup()
{
    //-- This does nothing (right now)
    RD_INFO("RdCameraBase: setup!\n");
    return true;
}

bool rdlib::RdCameraBase::start()
{
    //-- Start the capture thread
    int result = pthread_create( &capture_thread, NULL, captureThread, (void *) this );
    if (result == 0)
        {RD_INFO("RdCameraBase started thread.\n");}
    else
        {RD_WARNING("RdCameraBase could not create thread.\n");}

    return true;
}

bool rdlib::RdCameraBase::askToStop()
{
    RD_INFO("RdCameraBase: stopping...\n");
    stopThread = true;
    return true;
}

bool rdlib::RdCameraBase::quit()
{
    RD_INFO("RdCameraBase: exiting...\n");
    pthread_join( capture_thread, NULL);
    return true;
}

void rdlib::RdCameraBase::setRdManagerBasePtr(RdManagerBase* rdManagerBasePtr )
{
    this->rdManagerBasePtr = rdManagerBasePtr;
}

void rdlib::RdCameraBase::setCaptureSemaphores(sem_t *captureSemaphores)
{
    this->captureSemaphores = captureSemaphores;
}

void rdlib::RdCameraBase::setProcessSemaphores(sem_t *processSemaphores)
{
    this->processSemaphores = processSemaphores;
}

void *rdlib::RdCameraBase::captureThread(void *This)
{
    (( rdlib::RdCameraBase *) This )->captureWithSync();
}

void *rdlib::RdCameraBase::captureWithSync()
{
    if( !stopThread )
    {
        do
        {
            for (int i = 0; i < PIPELINE_SIZE; i++)
            {
            //-- Lock the semaphore
            sem_wait( captureSemaphores+i);

            //-- Capture frame however you want
            //RD_INFO("Captured frame #%i\n", i);
            this->capture(i);

            //-- Unlock the corresponding process semaphore
            sem_post( processSemaphores+i);
            }
        } while ( !stopThread);
    }

    //-- Lock twice the semaphore in order to be the last thread to exit in
    //-- the pipeline. This way the semaphores are deleted correctly and
    //-- the other threads are not locked
    //sem_wait(captureSemaphores);
    RD_SUCCESS("Exited camera main thread!");
}
