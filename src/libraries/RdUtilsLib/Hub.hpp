
// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __MANAGER_HUB_HPP__
#define __MANAGER_HUB_HPP__

#include "RdImageManager.hpp"
#include "RdAudioManager.hpp"
#include "RdInputManager.hpp"
#include "RdMentalMap.hpp"
#include "RdNetworkManager.hpp"
#include "RdRobotManager.hpp"

namespace rd{

/**
 * @ingroup RdStateMachineLib
 *
 * @brief A classs to interface all the robot devastation managers
 *
 */
class ManagerHub
{
    public:
        ManagerHub()
        {
            networkManager = NULL;
            imageManager = NULL;
            inputManager = NULL;
            mentalMap = NULL;
            robotManager = NULL;
        }

        ManagerHub(RdNetworkManager * networkManager, RdImageManager * imageManager, RdInputManager * inputManager, RdMentalMap * mentalMap, RdRobotManager * robotManager)
        {
            this->networkManager = networkManager;
            this->imageManager = imageManager;
            this->inputManager = inputManager;
            this->mentalMap = mentalMap;
            this->robotManager = robotManager;
        }

        bool setNetworkManager(RdNetworkManager * networkManager) { this->networkManager = networkManager;}
        bool setImageManager(RdImageManager * imageManager) { this->imageManager = imageManager;}
        bool setInputManager(RdInputManager * inputManager) { this->inputManager = inputManager;}
        bool setMentalMap(RdMentalMap * mentalMap) { this->mentalMap = mentalMap;}
        bool setRobotManager(RdRobotManager * robotManager) { this->robotManager = robotManager;}

   protected:
        //-- Pointers to everything important in the game:
        RdNetworkManager * networkManager;
        RdImageManager * imageManager;
        RdInputManager * inputManager;
        RdMentalMap * mentalMap;
        RdRobotManager * robotManager;

};
}

#endif //-- __MANAGER_HUB_HPP__
