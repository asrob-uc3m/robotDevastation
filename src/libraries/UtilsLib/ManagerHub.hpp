// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_MANAGER_HUB_HPP__
#define __RD_MANAGER_HUB_HPP__

#include "ImageManager.hpp"
#include "InputManager.hpp"
#include "MentalMap.hpp"
#include "NetworkManager.hpp"
#include "IRobotManager.hpp"
#include "AudioManager.hpp"
#include "ScreenManager.hpp"

namespace rd{

/**
 * @ingroup UtilsLib
 *
 * @brief A classs to interface all the robotDevastation managers. As some
 * classes, such as the game states, require require interfacing with all the
 * managers to access their functionallity, this class was created. It
 * simplifies the code, as by simple inheritance any class can have simple
 * access to all the required managers.
 *
 */
class ManagerHub
{
    public:
    ManagerHub();

    ManagerHub(NetworkManager * networkManager, ImageManager * imageManager,
               InputManager * inputManager, MentalMap * mentalMap,
               asrob::IRobotManager * robotManager, AudioManager * audioManager,
               ScreenManager * screenManager);

        bool setNetworkManager(NetworkManager * networkManager);
        bool setImageManager(ImageManager * imageManager);
        bool setInputManager(InputManager * inputManager);
        bool setMentalMap(MentalMap * mentalMap);
        bool setRobotManager(asrob::IRobotManager * robotManager);
        bool setAudioManager(AudioManager * audioManager);
        bool setScreenManager(ScreenManager * screenManager);

   protected:
        //-- Pointers to everything important in the game:
        NetworkManager * networkManager;
        ImageManager * imageManager;
        InputManager * inputManager;
        MentalMap * mentalMap;
        asrob::IRobotManager * robotManager;
        AudioManager * audioManager;
        ScreenManager * screenManager;
};

}

#endif //-- __RD_MANAGER_HUB_HPP__
