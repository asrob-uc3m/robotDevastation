// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_CAMERA_BASE_HPP__
#define __RD_CAMERA_BASE_HPP__

#include <iostream>
#include <pthread.h>
#include <semaphore.h>

#include "RdManagerBase.hpp"

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdCameraLib
 *
 * @brief The RdCameraLib library contains the \ref RdCameraBase class and some inherited implementations.
 */

namespace rdlib{

class RdManagerBase;
class RdRobotBase;

/**
 * @ingroup RdCameraBase
 *
 * @brief An abstract base class for Robot Devastation camera inputs (webcams, stereoscopic cams, etc).
 *
 */
class RdCameraBase {
    public:
        virtual bool start();
        virtual bool quit() = 0;
        virtual bool getDimensions( int& width, int& height, int &step) = 0;
        virtual char * getBufferPtr( int index = 0 ) = 0;

        void setRdManagerBasePtr(RdManagerBase* rdManagerBasePtr );

        //-- Get references to the semaphores
        sem_t * getCaptureSemaphores();
        sem_t * getProcessSemaphores();
        sem_t * getDisplaySemaphores();

        bool setStop( bool stop);

    protected:
        static const int PIPELINE_SIZE = 3;

        //-- Index is the index in the pipeline ( between 0 and PIPELINE_SIZE-1)
        virtual void capture(int index) = 0;

        //-- Pointers to other modules:
        RdManagerBase* rdManagerBasePtr;

        //-- Semaphores for camera/manager/output sync
        sem_t * captureSemaphores;
        sem_t * processSemaphores;
        sem_t * displaySemaphores;

        //-- Thread-related
        pthread_t capture_thread;
        bool stopThread;

    private:
        static void * captureThread( void * This );
        void *captureWithSync();



};

} //rdlib

#endif  // __RD_CAMERA_BASE_HPP__
