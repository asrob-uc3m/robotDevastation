#include "RdCameraBase.hpp"

bool rdlib::RdCameraBase::start()
{
    //-- Start the capture thread
    pthread_create( &capture_thread, NULL, captureThread, (void *) this );
}

void rdlib::RdCameraBase::setRdManagerBasePtr(RdManagerBase* rdManagerBasePtr )
{
    this->rdManagerBasePtr = rdManagerBasePtr;
}

sem_t *rdlib::RdCameraBase::getCaptureSemaphores()
{
    return captureSemaphores;
}

sem_t *rdlib::RdCameraBase::getProcessSemaphores()
{
    return processSemaphores;
}

sem_t *rdlib::RdCameraBase::getDisplaySemaphores()
{
    return displaySemaphores;
}

bool rdlib::RdCameraBase::setStop(bool stop)
{
     this->stopThread = stop;
}

void *rdlib::RdCameraBase::captureThread(void *This)
{
    (( rdlib::RdCameraBase *) This )->captureWithSync();
}

void *rdlib::RdCameraBase::captureWithSync()
{
    while( !stopThread )
    {
        for (int i = 0; i < PIPELINE_SIZE; i++)
        {
            //-- Lock the semaphore
            sem_wait( captureSemaphores+i);

            //-- Cpature frame however you want
            this->capture(i);

            //-- Unlock the corresponding process semaphore
            sem_post( processSemaphores+i);

        }
    }

}
