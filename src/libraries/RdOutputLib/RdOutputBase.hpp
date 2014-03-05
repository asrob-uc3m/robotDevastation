// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_OUTPUT_BASE_HPP__
#define __RD_OUTPUT_BASE_HPP__

#include <iostream>
#include <unistd.h>
#include "RdManagerBase.hpp"

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdInputLib
 *
 * @brief The RdInputLib library contains the \ref RdInputBase class and some inherited implementations.
 */

namespace rdlib{

class RdManagerBase;
class RdRobotBase;

/**
 * @ingroup RdOuputLib
 *
 * @brief An abstract base class for Robot Devastation ouputs (screen...).
 *
 */
class RdOutputBase {
    public:

        virtual bool start();

        /** A quit rutine.
         * @return true if the object was quit successfully.
         */
        virtual bool quit();

        void setRdManagerBasePtr(RdManagerBase* rdManagerBasePtr );

        void setDisplaySemaphores( sem_t * displaySemaphores);
        void setCaptureSemaphores( sem_t * captureSemaphores);

        bool setStop( bool stop );

    protected:
        static const int PIPELINE_SIZE = 3;

        virtual bool output(int pipelineIndex) = 0;

        //-- Sync with other modules
        sem_t * captureSemaphores;
        sem_t * displaySemaphores;

        //-- References to other modules
        RdCameraBase* rdCameraBasePtr;
        RdManagerBase* rdManagerBasePtr;

        //-- Thread-related
        pthread_t output_thread;
        bool isRunning;

   private:
        static void * outputThread( void * This );
        bool outputWithSync();


};

} //rdlib

#endif  // __RD_OUTPUT_BASE_HPP__

