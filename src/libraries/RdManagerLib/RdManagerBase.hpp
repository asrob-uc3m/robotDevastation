// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MANAGER_BASE_HPP__
#define __RD_MANAGER_BASE_HPP__

#include "RdMacros.hpp"

#include "RdInputBase.hpp"
#include "RdOutputBase.hpp"
#include "RdRobotBase.hpp"
#include "RdCameraBase.hpp"

#include <iostream>
#include <vector>
#include <utility>
#include <pthread.h>
#include <semaphore.h>

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdManagerLib
 *
 * @brief The RdManagerLib library contains the \ref RdManagerBase class and some inherited implementations.
 */

namespace rdlib{

class RdCameraBase;
class RdInputBase;
class RdOutputBase;
class RdRobotBase;

/**
 * @ingroup RdManagerLib
 *
 * @brief An abstract base class for Robot Devastation managers.
 *
 */
class RdManagerBase {
    public:

        RdManagerBase();

        virtual bool callFunctionByName(const std::string& cmd);

        virtual bool setup();
        virtual bool start();

        virtual bool askToStop();
        static bool askToStopWrapper( void * This);

        /** A quit rutine.
         * @return true if the object was quit successfully.
         */
        virtual bool quit();

        /** An shoot rutine.
         * @return true if successful.
         */
        virtual bool shoot() = 0;
        static bool shootWrapper(void *This);

        int getManagerStatus();
        RdCameraBase* getRdCameraBasePtr();
        void getEnemies(int pipelineIndex,  std::vector< std::pair<int, int> >& enemyPos, std::vector< double >& enemySize);

        void setRdCameraBasePtr(RdCameraBase* rdCameraBasePtr );
        void setRdInputBasePtr(RdInputBase* rdInputBasePtr ) ;
        void setRdOutputBasePtr(RdOutputBase* rdOutputBasePtr );
        void setRdRobotBasePtr(RdRobotBase* rdRobotBasePtr );

        static const int MANAGER_STATUS_OK = 0;
        static const int MANAGER_STATUS_STOPPED = -1;

    protected:
        static const int PIPELINE_SIZE = 3;

        int managerStatus;
        std::map< std::string, void*> functionMap;

        //-- Pointers to other modules
        RdCameraBase* rdCameraBasePtr;
        RdInputBase* rdInputBasePtr;
        RdOutputBase* rdOutputBasePtr;
        RdRobotBase* rdRobotBasePtr;

        //-- Thread-related
        pthread_t processImage_thread;

        //-- Semaphores for camera/manager/output sync
        sem_t * captureSemaphores;
        sem_t * processSemaphores;
        sem_t * displaySemaphores;

        //-- Enemies
        std::vector< std::vector< std::pair<int, int> > > enemyPos;
        std::vector< std::vector< double > > enemySize;

        virtual bool processImage() = 0;

     private:
        static void * processImageThread(void * This);
};

} //rdlib

#endif  // __RD_MANAGER_BASE_HPP__

