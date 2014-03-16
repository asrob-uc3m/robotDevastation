#include "RdCameraBase.hpp"

bool rdlib::RdCameraBase::start()
{
    //-- Start the capture thread
    pthread_create( &capture_thread, NULL, captureThread, (void *) this );
}

bool rdlib::RdCameraBase::quit()
{
    std::cout << "[info] RdCameraBase quit()" << std::endl;
    stopThread = true;
    pthread_join( capture_thread, NULL);

    delete[] captureSemaphores;
    captureSemaphores = 0;
    delete[] processSemaphores; //-- Doing this here will cause (probably) a segmentation fault
    processSemaphores = 0;
    delete[] displaySemaphores;
    displaySemaphores = 0;
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
    if( !stopThread )
    {
        do
        {
            for (int i = 0; i < PIPELINE_SIZE; i++)
            {
            //-- Lock the semaphore
            sem_wait( captureSemaphores+i);

            //-- Capture frame however you want
            std::cout << "[info] Captured frame #" << i << std::endl;
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
