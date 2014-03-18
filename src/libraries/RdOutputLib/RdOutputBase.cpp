#include "RdOutputBase.hpp"

rdlib::RdOutputBase::RdOutputBase()
{
    stopThread=false;
}

bool rdlib::RdOutputBase::setup()
{
    //-- This does nothing (right now)
    RD_INFO("RdOutputBase: setup!\n");
}

bool rdlib::RdOutputBase::start()
{
    if ( rdManagerBasePtr == NULL)
    {
        RD_ERROR("RdOutputBase could not start. Missing pointer to manager.\n");
        return false;
    }
    rdCameraBasePtr = rdManagerBasePtr->getRdCameraBasePtr();

    if ( ! rdCameraBasePtr )
    {
        RD_ERROR("RdOutputBase could not start. Missing pointer to camera.\n");
        return false;
    }

    //-- Start the display thread
    int result = pthread_create( &output_thread, NULL, outputThread, (void *) this );
    if (result == 0)
        {RD_INFO("RdOutputBase started thread.\n");}
    else
        {RD_WARNING("RdOutputBase could not create thread.\n");}

    return true;
}

bool rdlib::RdOutputBase::askToStop()
{
    RD_INFO("RdOutputBase: stopping...\n");
    stopThread = true;
    return true;
}

bool rdlib::RdOutputBase::quit()
{
    RD_INFO("RdOutputBase: exiting...\n");
    pthread_join( output_thread, NULL);
    return true;
}


void rdlib::RdOutputBase::setRdManagerBasePtr(rdlib::RdManagerBase *rdManagerBasePtr)
{
    this->rdManagerBasePtr = rdManagerBasePtr;
}

void rdlib::RdOutputBase::setDisplaySemaphores(sem_t *displaySemaphores)
{
    this->displaySemaphores = displaySemaphores;
}

void rdlib::RdOutputBase::setCaptureSemaphores(sem_t *captureSemaphores)
{
    this->captureSemaphores = captureSemaphores;
}

void *rdlib::RdOutputBase::outputThread(void *This)
{
    ((RdOutputBase*) This)->outputWithSync();
}

bool rdlib::RdOutputBase::outputWithSync()
{
    if (!stopThread)
    {
        do
        {
            for (int i = 0; i < PIPELINE_SIZE; i++)
            {
                //-- Lock the semaphore
                sem_wait(displaySemaphores+i);

                //-- Output screen
                //RD_INFO("Output frame #%i\n", i);
                this->output(i);

                //-- Unlock the corresponding process semaphore
                sem_post(captureSemaphores+i);
            }
        } while (!stopThread);
    }

    //sem_post(captureSemaphores);
    RD_SUCCESS("Exited output main thread!");
}
