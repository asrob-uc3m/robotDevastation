#include "RdOutputBase.hpp"

rdlib::RdOutputBase::RdOutputBase()
{
    isRunning=true;
}

bool rdlib::RdOutputBase::start()
{
    if ( rdManagerBasePtr == NULL)
    {
        std::cerr << "[error] RdOutputBase could not start. Missing pointer to manager.";
        std::cerr << std::endl;
        return false;
    }
    rdCameraBasePtr = rdManagerBasePtr->getRdCameraBasePtr();

    if ( ! rdCameraBasePtr )
    {
        std::cerr << "[error] RdOutputBase could not start. Missing pointer to camera.";
        std::cerr << std::endl;
        return false;
    }

    //-- Start the display thread
    pthread_create( &output_thread, NULL, outputThread, (void *) this );
    std::cout << "[info] RdOutputBase created thread." << std::endl;
}

bool rdlib::RdOutputBase::quit()
{
    std::cout << "[info] RdOutputHighgui quit()" << std::endl;
    isRunning = false;
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
    ((RdOutputBase* ) This)->outputWithSync();
}

bool rdlib::RdOutputBase::outputWithSync()
{
    while( isRunning )
    {
        for (int i = 0; i < PIPELINE_SIZE; i++)
        {
            //-- Lock the semaphore
            sem_wait( displaySemaphores+i);

            //-- Output screen
            //std::cout << "[info] Output frame #" << i << std::endl;
            this->output(i);

            //-- Unlock the corresponding process semaphore
            sem_post( captureSemaphores+i);

        }
    }
}
