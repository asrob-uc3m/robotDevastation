// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "RobotDevastation.hpp"

#include <cstdio>

#include <yarp/os/LogComponent.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Property.h>

namespace
{
    YARP_LOG_COMPONENT(RD, "asrob.rd.RobotDevastation")
}

bool rd::RobotDevastation::configure(yarp::os::ResourceFinder &rf)
{
    //-- Get player data
    //-----------------------------------------------------------------------------
    //-- Request player/robot info
    if( ! initPlayerInfo(rf) )
        return false;


    //-- Init screen manager
    if( ! SDLScreenManager::RegisterManager() )
        return false;
    screenManager = ScreenManager::getScreenManager(SDLScreenManager::id);
    if (screenManager==NULL)
    {
        yCError(RD) << "Could not create ScreenManager";
        return false;
    }
    if( rf.check("fullscreen") )
        screenManager->configure(SDLScreenManager::PARAM_FULLSCREEN, "enabled");

    screenManager->start();

    //-- Init input manager
    if( ! SDLInputManager::RegisterManager() )
        return false;
    inputManager = InputManager::getInputManager("SDL");

    //-- Init sound
    if( ! initSound(rf) )
        return false;

    //-- Configure robot device
    yarp::os::Property robotOptions;
    if( rf.check("fakeRobotManager") )
        robotOptions.put("device", "FakeMotorController");
    else
        robotOptions.put("device", "RobotClient");
    robotOptions.put("name", "/" + robotName);

    //-- Start robot device
    if( ! robotDevice.open(robotOptions) )
    {
        yCError(RD) << "Found no robot motors with robotName" << robotName << "(try running with --fakeRobotManager if no robot)";
        return false;
    }

    //-- Acquire robot interface
    if( ! robotDevice.view(robotManager) )
    {
        yCError(RD) << "Could not acquire robot interface";
        return false;
    }

    //-- Init image manager
    if( rf.check("fakeImageManager") )
    {
        if( ! MockImageManager::RegisterManager() )
            return false;
        imageManager = ImageManager::getImageManager(MockImageManager::id);
    }
    else if( rf.check("yarpLocalImageManager") )
    {
        if( ! YarpLocalImageManager::RegisterManager() )
            return false;
        imageManager = ImageManager::getImageManager(YarpLocalImageManager::id);
        if (imageManager == NULL)
        {
            yCError(RD) << "Could not create yarpLocalImageManager";
            return false;
        }

        if(rf.check("camera_id"))
        {
            std::stringstream camera_id_ss;
            camera_id_ss << rf.find("camera_id").asInt32();
            yCInfo(RD) << "YarpLocalImageManager is using camera with index" << camera_id_ss.str();
            imageManager->configure("camera_id", camera_id_ss.str());
        }
    }
    else
    {
        if( ! YarpImageManager::RegisterManager() )
            return false;
        imageManager = ImageManager::getImageManager(YarpImageManager::id);
    }

    //-- Configure the camera port
    std::ostringstream remoteCameraPortName;  //-- Default looks like "/rd1/img:o"
    remoteCameraPortName << "/";
    remoteCameraPortName << robotName;
    remoteCameraPortName << "/img:o";
    imageManager->configure("remote_img_port", remoteCameraPortName.str() );
    std::ostringstream localCameraPortName;  //-- Default should look like "/robotDevastation/rd1/img:i"
    localCameraPortName << "/robotDevastation/";
    localCameraPortName << robotName;
    localCameraPortName << "/img:i";
    imageManager->configure("local_img_port", localCameraPortName.str() ); //-- Name given by me

    //-- Init mentalMap
    mentalMap = MentalMap::getMentalMap();
    mentalMap->configure(id);

    std::vector<Player> players;
    players.push_back(Player(id,name,100,100,team,0));
    mentalMap->updatePlayers(players);

    mentalMap->addWeapon(Weapon("Default gun", 10, 5));

    //-- Init network manager
    if( ! YarpNetworkManager::RegisterManager() )
        return false;
    networkManager = NetworkManager::getNetworkManager(YarpNetworkManager::id);
    networkManager->configure("player", players[0]);

    //-- Get game FSM and start game
    if( ! initGameFSM() )
        return false;

    if( ! gameFSM->start() )
        return false;

    return true;
}

double rd::RobotDevastation::getPeriod()
{
    return 1;  // Fixed, in seconds, the slow thread that calls updateModule below
}

bool rd::RobotDevastation::updateModule()
{
    //-- Check if FSM has arrived the end-state
    if (gameFSM->getCurrentState() == -1)
    {
        stopModule();
        return true;
    }

    if (mentalMap != NULL)
    {
        std::printf("===robotDevastation===\n");
        std::printf("Number of players: %zd\n",mentalMap->getPlayers().size());
        for(size_t i=0;i<mentalMap->getPlayers().size();i++)
        {
            std::printf("----------------------\n%s\n",mentalMap->getPlayers().at(i).str().c_str());
        }
        //std::printf("======================\n");
        return true;
    }

    yCDebug(RD) << "Current state id:" << gameFSM->getCurrentState();
    return true;
}

bool rd::RobotDevastation::initPlayerInfo(yarp::os::ResourceFinder &rf)
{
    if(rf.check("id"))
    {
        id = rf.find("id").asInt32();
        std::stringstream question;
        question << "Insert player id [" << id << "]";
        getInfoFromUser(question.str(), id);
    }
    else
    {
        id = -1;
        getInfoFromUser("Insert player id []", id, true);
    }


    if(rf.check("name"))
    {
        name = rf.find("name").asString();
        std::stringstream question;
        question << "Insert name [" << name << "]";
        getInfoFromUser(question.str(), name);
    }
    else
    {
        name = "";
        getInfoFromUser("Insert name []", name, true);
    }

    if(rf.check("team"))
    {
        team = rf.find("team").asInt32();
        std::stringstream question;
        question << "Insert team [" << team << "]";
        getInfoFromUser(question.str(), team);
    }
    else
    {
        team = -1;
        getInfoFromUser("Insert team []", team, true);
    }

    if(rf.check("robotName"))
    {
        robotName = rf.find("robotName").asString();
        std::stringstream question;
        question << "Insert robotName [" << robotName << "]";
        getInfoFromUser(question.str(), robotName);
    }
    else
    {
        robotName = "";
        getInfoFromUser("Insert robotName []", robotName, true);
    }

    std::printf("\n\n");
    std::printf("Player information:\n");
    std::printf("\t-id: %d\n", id);
    std::printf("\t-name: %s\n", name.c_str());
    std::printf("\t-team: %d\n", team);
    std::printf("\t-robotName: %s\n", robotName.c_str());
    return true;
}

bool rd::RobotDevastation::initSound(yarp::os::ResourceFinder &rf)
{
    if( ! SDLAudioManager::RegisterManager() )
        return false;
    audioManager = AudioManager::getAudioManager("SDL");

    std::string bsoStr( rf.findFileByName("../sounds/RobotDevastationBSO.mp3") );
    if ( ! audioManager->load(bsoStr, "RD_THEME", 0) )
        return false;

    std::string shootStr( rf.findFileByName("../sounds/01_milshot.wav") );
    if ( ! audioManager->load(shootStr, "shoot", 1) )
        return false;

    std::string explosionStr( rf.findFileByName("../sounds/15_explosion.wav") );
    if ( ! audioManager->load(explosionStr, "explosion", 1) )
        return false;

    std::string noAmmoStr( rf.findFileByName("../sounds/03_clip.wav") );
    if ( ! audioManager->load(noAmmoStr, "noAmmo", 1) )
        return false;

    std::string reloadStr( rf.findFileByName("../sounds/04_milaction.wav") );
    if ( ! audioManager->load(reloadStr, "reload", 1) )
        return false;

    return true;
}

bool rd::RobotDevastation::initGameFSM()
{
    StateMachineBuilder builder;
    builder.setDirectorType("YARP");

    //-- Create states
    int init_state_id = builder.addState(new InitState(networkManager, imageManager, inputManager, mentalMap,
                                                       robotManager, audioManager, screenManager));
    int game_state_id = builder.addState(new GameState(networkManager, imageManager, inputManager, mentalMap,
                                                       robotManager, audioManager, screenManager));
    int dead_state_id = builder.addState(new DeadState(networkManager, imageManager, inputManager, mentalMap,
                                                       robotManager, audioManager, screenManager));
    int end_state_id = builder.addState(State::getEndState());

    //-- Add transitions to other states
    builder.addTransition(init_state_id, game_state_id, InitState::LOGIN_SUCCESSFUL);
    builder.addTransition(init_state_id, end_state_id, InitState::EXIT_REQUESTED);
    builder.addTransition(game_state_id, dead_state_id, GameState::KILLED);
    builder.addTransition(game_state_id, end_state_id, GameState::EXIT_REQUESTED);
    builder.addTransition(dead_state_id, game_state_id, DeadState::RESPAWN_SELECTED);
    builder.addTransition(dead_state_id, end_state_id, DeadState::EXIT_SELECTED);

    //-- Set initial state
    builder.setInitialState(init_state_id);
    gameFSM = builder.buildStateMachine();

    return gameFSM != NULL;
}

bool rd::RobotDevastation::cleanup()
{
    InputManager::destroyInputManager();
    inputManager = NULL;

    NetworkManager::destroyNetworkManager();
    networkManager = NULL;

    //-- Closing audio system:
    AudioManager::destroyAudioManager();
    audioManager = NULL;

    //-- Closing mental map:
    MentalMap::destroyMentalMap();
    mentalMap = NULL;

    //-- Close img related ports:
    ImageManager::destroyImageManager();
    imageManager = NULL;

    //-- Close robot:
    robotDevice.close();

    //-- Delete FSM:
    delete gameFSM;
    gameFSM = NULL;

    //-- Close SDL / GUI
    screenManager->stop();
    ScreenManager::destroyScreenManager();
    screenManager = NULL;

    return true;
}

bool rd::RobotDevastation::interruptModule()
{
    yCInfo(RD) << "Closing program...";
    return cleanup();
}
