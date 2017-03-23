// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __ROBOT_DEVASTATION_HPP__
#define __ROBOT_DEVASTATION_HPP__

#include <iostream>
#include <string>
#include <sstream>

#include <yarp/os/RFModule.h>

#include "Utils.hpp"
#include "SDLAudioManager.hpp"
#include "MentalMap.hpp"
#include "InputManager.hpp"
#include "SDLInputManager.hpp"
#include "InputEventListener.hpp"
#include "YarpNetworkManager.hpp"
#include "RobotManager.hpp"
#include "YarpRobotManager.hpp"
#include "MockupRobotManager.hpp"
#include "StateMachine.hpp"
#include "ImageManager.hpp"
#include "YarpImageManager.hpp"
#include "YarpLocalImageManager.hpp"
#include "MockupImageManager.hpp"
#include "ProcessorImageEventListener.hpp"
#include "SDLScreenManager.hpp"

//-- Game FSM
#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "InitState.hpp"
#include "GameState.hpp"
#include "DeadState.hpp"

namespace rd
{

/**
 * @ingroup robotDevastation
 * @brief The parent Robot Devastation class of the \ref robotDevastation program.
 */
class RobotDevastation : public yarp::os::RFModule
{
    public:
        /** Called on initialization. */
        virtual bool configure(yarp::os::ResourceFinder &rf);

    private:

        template<typename T> void getInfoFromUser(const std::string& question, T& info, const bool& no_empty=false)
        {

            std::string answer;
            do {
                printf("%s?>",question.c_str());
                std::getline(std::cin, answer);
            } while(no_empty && answer.empty());

            if (!answer.empty())
            {
                std::stringstream ss(answer);
                ss >> info;
            }
        }

        FiniteStateMachine * gameFSM;

        InputManager *  inputManager;
        AudioManager * audioManager;
        MentalMap * mentalMap;
        NetworkManager * networkManager;
        RobotManager * robotManager;
        ImageManager * imageManager;
        ScreenManager * screenManager;

        bool interruptModule();
        double getPeriod();
        bool updateModule();

        bool initPlayerInfo(yarp::os::ResourceFinder &rf);
        bool initSound(yarp::os::ResourceFinder &rf);
        bool initGameFSM();
        bool cleanup();

        //-- Player data
        int id;
        std::string name;
        int team;
        std::string robotName;
};

}  // namespace rd

#endif  // __ROBOT_DEVASTATION_HPP__

