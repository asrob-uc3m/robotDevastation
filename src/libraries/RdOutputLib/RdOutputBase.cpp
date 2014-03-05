#include "RdOutputBase.hpp"

bool rdlib::RdOutputBase::start()
{
    //-- Start the display thread
    pthread_create( &output_thread, NULL, outputThread, (void *) this );
    std::cout << "[info] RdOutputBase created thread." << std::endl;
}

bool rdlib::RdOutputBase::quit()
{
    std::cout << "[info] RdOutputHighgui quit()" << std::endl;
    pthread_join( output_thread, NULL);
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

            //-- Cpature frame however you want
            this->output(i);

            //-- Unlock the corresponding process semaphore
            sem_post( captureSemaphores+i);

        }
    }
}
