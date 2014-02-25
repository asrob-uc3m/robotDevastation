// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdManagerDefault.hpp"

rdlib::RdManagerDefault::RdManagerDefault()
{
    std::cout << "[info] RdManagerDefault constructor." << std::endl;
    this->managerStatus = 0;

    //-- Add functions (currently only hardcoded)
    functionMap["toggleHeadTrack"] = (void *) &toggleHeadTrackWrapper;
}

bool rdlib::RdManagerDefault::start()
{
    //-- Start the capture thread
    pthread_create( &processImage_thread, NULL, processImageThread, (void *) this );
}

bool rdlib::RdManagerDefault::shoot() {
    if (!rdRobotBasePtr) return false;
    //
    return rdRobotBasePtr->shoot();
}

void * rdlib::RdManagerDefault::processImageThread(void *This)
{
    (( rdlib::RdManagerDefault *) This)->processImage();
}

bool rdlib::RdManagerDefault::processImage()
{
    while( managerStatus != -1 )
    {
        for (int i = 0; i < 3; i++)
        {
            //-- Lock the semaphore
            sem_wait( processSemaphores+i);

            //-- Right now, it doesn't do much
            //std::cout << "[info] Processed frame # " << i << "." << std::endl;
            if ( managerStatus == 1 )
                std::cout << "[info] Tracking head." << std::endl;

            //-- Unlock the corresponding process semaphore
            sem_post( displaySemaphores+i);
        }
    }
}

bool rdlib::RdManagerDefault::toggleHeadTrackWrapper(void *This)
{
       (( rdlib::RdManagerDefault *) This)->toggleHeadTrack();
}

bool rdlib::RdManagerDefault::toggleHeadTrack()
{
    if ( managerStatus != 1 )
        managerStatus = 1;
    else
        managerStatus = 0;
}

bool rdlib::RdManagerDefault::quit()
{
    std::cout << "[info] RdManagerDefault quit." << std::endl;
    this->managerStatus = -1;
    pthread_join( processImage_thread, NULL );
    return true;
}

void rdlib::RdManagerDefault::setProcessSemaphores(sem_t *processSemaphores)
{
    this->processSemaphores = processSemaphores;
}

void rdlib::RdManagerDefault::setDisplaySemaphores(sem_t *displaySemaphores)
{
    this->displaySemaphores = displaySemaphores;
}

