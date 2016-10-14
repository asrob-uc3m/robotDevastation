// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RobotDevastation.hpp"

bool rd::RobotDevastation::configure(yarp::os::ResourceFinder &rf)
{
    //-- Get player data
    //-----------------------------------------------------------------------------
    //-- Request player/robot info
    if( ! initPlayerInfo(rf) )
        return false;

    //-- Init graphics
    if( ! initSDL() )
        return false;

    //-- Init input manager
    RdSDLInputManager::RegisterManager();
    inputManager = RdInputManager::getInputManager("SDL");

    //-- Init sound
    if(!initSound(rf))
        return false;

    //-- Init robot
    if(rf.check("mockupRobotManager"))
        robotManager = new RdMockupRobotManager(rf.find("robotName").asString());
    else
        robotManager = new RdYarpRobotManager(rf.find("robotName").asString());

    //-- Init image manager
    if(rf.check("mockupImageManager"))
    {
        RdMockupImageManager::RegisterManager();
        imageManager = RdImageManager::getImageManager(RdMockupImageManager::id);
    }
    else if(rf.check("yarpLocalImageManager"))
    {
        RdYarpLocalImageManager::RegisterManager();
        imageManager = RdImageManager::getImageManager(RdYarpLocalImageManager::id);
    }
    else
    {
        RdYarpImageManager::RegisterManager();
        imageManager = RdImageManager::getImageManager(RdYarpImageManager::id);
    }

    //-- Configure the camera port
    std::ostringstream remoteCameraPortName;  //-- Default looks like "/rd1/img:o"
    remoteCameraPortName << "/";
    remoteCameraPortName << rf.find("robotName").asString();
    remoteCameraPortName << "/img:o";
    imageManager->configure("remote_img_port", remoteCameraPortName.str() );
    std::ostringstream localCameraPortName;  //-- Default should look like "/robotDevastation/rd1/img:i"
    localCameraPortName << "/robotDevastation/";
    localCameraPortName << rf.find("robotName").asInt();
    localCameraPortName << "/img:i";
    imageManager->configure("local_img_port", localCameraPortName.str() ); //-- Name given by me

    //-- Init mentalMap
    mentalMap = RdMentalMap::getMentalMap();
    mentalMap->configure(rf.find("id").asInt());

    std::vector<RdPlayer> players;
    players.push_back(RdPlayer(rf.find("id").asInt(),std::string(rf.find("name").asString()),100,100,rf.find("team").asInt(),0));
    mentalMap->updatePlayers(players);

    mentalMap->addWeapon(RdWeapon("Default gun", 10, 5));

    //-- Init network manager
    RdYarpNetworkManager::RegisterManager();
    networkManager = RdYarpNetworkManager::getNetworkManager(RdYarpNetworkManager::id);
    networkManager->configure("player", players[0]);

    //-- Get game FSM and start game
    if(initGameFSM())
        gameFSM->start();

    return true;
}

double rd::RobotDevastation::getPeriod()
{
    return 2.0;  // Fixed, in seconds, the slow thread that calls updateModule below
}

bool rd::RobotDevastation::updateModule()
{
    //-- Check if FSM has arrived the end-state
    if (gameFSM->getCurrentState() == -1)
    {
        this->stopModule();
        return true;
    }

    if (mentalMap != NULL)
    {
        printf("===robotDevastation===\n");
        printf("Number of players: %zd\n",mentalMap->getPlayers().size());
        for(size_t i=0;i<mentalMap->getPlayers().size();i++)
        {
           printf("----------------------\n%s\n",mentalMap->getPlayers().at(i).str().c_str());
        }
        //printf("======================\n");
        return true;
    }

    RD_DEBUG("Current state id: %d\n", gameFSM->getCurrentState());
    return true;
}

bool rd::RobotDevastation::initPlayerInfo(yarp::os::ResourceFinder &rf)
{
    if(rf.check("id"))
    {
        id = rf.find("id").asInt();
        std::stringstream question;
        question << "Insert player id [" << id << "]";
        getInfoFromUser(question.str(), id);
    }
    else
    {
        id = -1;
        getInfoFromUser("Insert player id []", id, true);
    }


    if(!rf.check("name"))
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

    if(!rf.check("team"))
    {
        team = rf.find("team").asInt();
        std::stringstream question;
        question << "Insert team [" << team << "]";
        getInfoFromUser(question.str(), team);
    }
    else
    {
        team = -1;
        getInfoFromUser("Insert team []", team, true);
    }

    if(!rf.check("robotName"))
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

    printf("\n\n");
    printf("Player information:\n");
    printf("\t-id: %d\n", id);
    printf("\t-name: %s\n", name.c_str());
    printf("\t-team: %d\n", team);
    printf("\t-robotName: %s\n", robotName.c_str());
    return true;
}

bool rd::RobotDevastation::initSound(yarp::os::ResourceFinder &rf)
{
    SDLAudioManager::RegisterManager();
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
                                                       robotManager, audioManager));
    int game_state_id = builder.addState(new GameState(networkManager, imageManager, inputManager, mentalMap,
                                                       robotManager, audioManager));
    int dead_state_id = builder.addState(new DeadState(networkManager, imageManager, inputManager,
                                                       mentalMap, robotManager, audioManager));
    int end_state_id = builder.addState(State::getEndState());

    //-- Add transitions to other states
    builder.addTransition(init_state_id, game_state_id, InitState::LOGIN_SUCCESSFUL);
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
    RdInputManager::destroyInputManager();
    inputManager = NULL;

    RdNetworkManager::destroyNetworkManager();
    networkManager = NULL;

    //-- Closing audio system:
    AudioManager::destroyAudioManager();
    audioManager = NULL;

    //-- Closing mental map:
    RdMentalMap::destroyMentalMap();
    mentalMap = NULL;

    //-- Close img related ports:
    RdImageManager::destroyImageManager();
    imageManager = NULL;

    //-- Close robot:
    delete robotManager;
    robotManager = NULL;

    //-- Delete FSM:
    delete gameFSM;
    gameFSM = NULL;

    cleanupSDL();
    return true;
}

bool rd::RobotDevastation::interruptModule()
{
    RD_INFO("Closing program...\n");
    return this->cleanup();
}

