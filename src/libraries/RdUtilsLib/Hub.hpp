
// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __MANAGER_HUB_HPP__
#define __MANAGER_HUB_HPP__

#include "RdImageManager.hpp"
#include "SDLAudioManager.hpp" //-- Should be AudioManager.hpp
#include "RdInputManager.hpp"
#include "RdKey.hpp"
#include "RdMentalMap.hpp"
#include "RdWeapon.hpp"
#include "RdNetworkManager.hpp"
#include "RdRobotManager.hpp"

namespace rd{

/**
 * @ingroup RdUtils
 *
 * @brief A classs to interface all the robot devastation managers
 *
 */
class ManagerHub
{
    public:
    ManagerHub();

    ManagerHub(RdNetworkManager * networkManager, RdImageManager * imageManager,
               RdInputManager * inputManager, RdMentalMap * mentalMap, RdRobotManager * robotManager);

        bool setNetworkManager(RdNetworkManager * networkManager);
        bool setImageManager(RdImageManager * imageManager);
        bool setInputManager(RdInputManager * inputManager);
        bool setMentalMap(RdMentalMap * mentalMap);
        bool setRobotManager(RdRobotManager * robotManager);

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
