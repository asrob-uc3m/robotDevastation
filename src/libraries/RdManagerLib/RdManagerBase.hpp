// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MANAGER_BASE_HPP__
#define __RD_MANAGER_BASE_HPP__

#include "RdMacros.hpp"

#include "RdInputBase.hpp"
#include "RdOutputBase.hpp"
#include "RdRobotBase.hpp"
#include "RdCameraBase.hpp"
#include "RdWeaponBase.hpp"
#include "RdImageProcessBase.hpp"
#include "RdEnemy.hpp"

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
class RdImageProcessBase;
class RdNetworkBase;
class RdEnemy;

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
        virtual bool shoot();
        virtual bool reload();
        virtual bool changeWeapon(int index = -1);

        //-- Enemies -----------------------------------------------//
        void getEnemies( std::vector< rdlib::RdEnemy >& vectorOfRdEnemy, pthread_mutex_t& mutexOfVectorOfRdEnemy );

        //-- Weapons -----------------------------------------------//
        std::vector<RdWeaponBase> getWeapons();
        RdWeaponBase* getCurrentWeapon();
        int getCurrentWeaponIndex();

        bool updateWeapons();


        //-- Set pointers to other modules -------------------------//
        void setRdCameraBasePtr(RdCameraBase* rdCameraBasePtr );
        void setRdInputBasePtr(RdInputBase* rdInputBasePtr ) ;
        void setRdOutputBasePtr(RdOutputBase* rdOutputBasePtr );
        void setRdRobotBasePtr(RdRobotBase* rdRobotBasePtr );
        void setRdNetworkBasePtr(RdNetworkBase* rdNetworkBasePtr );

        //-- Different status of the manager ----------------------//
        int getManagerStatus();
        static const int MANAGER_STATUS_OK = 0;
        static const int MANAGER_STATUS_STOPPED = -1;

    protected:
        static const int PIPELINE_SIZE = 3;

        int managerStatus;

        //-- Pointers to other modules
        RdCameraBase* rdCameraBasePtr;
        RdInputBase* rdInputBasePtr;
        RdOutputBase* rdOutputBasePtr;
        RdRobotBase* rdRobotBasePtr;
        RdNetworkBase* rdNetworkBasePtr;

        RdImageProcessBase* rdImageProcessorBasePtr;

        //-- Thread-related
        pthread_t manage_thread;

        //-- Semaphores for camera/manager/output sync
        sem_t * captureSemaphores;
        sem_t * processSemaphores;
        sem_t * displaySemaphores;

        //-- Enemies
        std::vector<RdEnemy> vectorOfRdEnemy;
        pthread_mutex_t mutexOfVectorOfRdEnemy;

        //-- Weapons
        std::vector<RdWeaponBase> weapons;
        RdWeaponBase * currentWeapon;
        int currentWeaponIndex;

        virtual bool manage(int pipelineIndex) = 0;

     private:
        static void * manageThread(void * This);
        bool manageWithSync();
};

} //rdlib

#endif  // __RD_MANAGER_BASE_HPP__

