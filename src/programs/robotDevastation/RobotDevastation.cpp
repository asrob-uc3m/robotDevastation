// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RobotDevastation.hpp"

bool rd::RobotDevastation::configure(yarp::os::ResourceFinder &rf)
{
    initSDL();

    //-- Show help
    //printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("RobotDevastation mandatory parameters:\n");
        printf("\t--id integer\n");
        printf("\t--name string\n");
        printf("\t--team integer\n");
        printf("\t--robotName string\n");
        printf("RobotDevastation optional parameters:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--mockupRobotManager  //-- Fake robot motors\n");
        printf("\t--mockupImageManager  //-- Fake robot camera\n");
        ::exit(0);
    }

    //-- Get player data
    //-----------------------------------------------------------------------------
    if( ! rf.check("id") )
    {
        RD_ERROR("No id! Please invoke with this parameter, as in 'robotDevastation --id integer --name string --team integer --robotName string'.\n");
        RD_ERROR("Type 'robotDevastation --help' for help.\n");
        return false;
    }
    RD_INFO("id: %d\n",rf.find("id").asInt());

    if( ! rf.check("name") )
    {
        RD_ERROR("No name! Please invoke with this parameter, as in 'robotDevastation --id integer --name string --team integer --robotName string'.\n");
        RD_ERROR("Type 'robotDevastation --help' for help.\n");
        return false;
    }
    RD_INFO("name: %s\n",rf.find("name").asString().c_str());

    if( ! rf.check("team") )
    {
        RD_ERROR("No team! Please invoke with this parameter, as in 'robotDevastation --id integer --name string --team integer --robotName string'.\n");
        RD_ERROR("Type 'robotDevastation --help' for help.\n");
        return false;
    }
    RD_INFO("team: %d\n",rf.find("team").asInt());

    if( ! rf.check("robotName") )
    {
        RD_ERROR("No robotName! Please invoke with this parameter, as in 'robotDevastation --id integer --name string --team integer --robotName string'.\n");
        RD_ERROR("Type 'robotDevastation --help' for help.\n");
        return false;
    }
    RD_INFO("robotName: %s\n",rf.find("robotName").asString().c_str());


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

//    RD_ERROR("Could not connect to robotName \"%s\"!\n",rf.find("robotName").asString().c_str());
//    RD_ERROR("Use syntax: robotDevastation --robotName %s\n",rf.find("robotName").asString().c_str());

    //-- Init image manager
    if(rf.check("mockupImageManager"))
    {
        RdMockupImageManager::RegisterManager();
        imageManager = RdImageManager::getImageManager(RdMockupImageManager::id);
    }
    else
    {
        RdYarpImageManager::RegisterManager();
        imageManager = RdImageManager::getImageManager(RdYarpImageManager::id);
    }

    //-- Configure the camera port
    std::ostringstream remoteCameraPortName;  //-- Default looks like "/0/rd1/img:o"
    remoteCameraPortName << "/";
    remoteCameraPortName << rf.find("robotName").asString();
    remoteCameraPortName << "/img:o";
    imageManager->configure("remote_img_port", remoteCameraPortName.str() );
    std::ostringstream localCameraPortName;  //-- Default should look like "/0/robot/img:i"
    localCameraPortName << "/";
    localCameraPortName << rf.find("id").asInt();
    localCameraPortName << "/robot/img:i";
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

    return cleanup();
}

double rd::RobotDevastation::getPeriod()
{
    return 2.0;  // Fixed, in seconds, the slow thread that calls updateModule below
}

bool rd::RobotDevastation::updateModule()
{
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
    else
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

    cleanupSDL;
    return true;
}

bool rd::RobotDevastation::interruptModule()
{
    RD_INFO("Closing program...\n");

    //-- Detach listeners to avoid segmentation faults (just in case)
    inputManager->removeInputEventListeners();
    networkManager->removeNetworkEventListeners();
    mentalMap->removeMentalMapEventListeners();
    imageManager->removeImageEventListeners();

    //-- Closing input manager:
    RdInputManager::destroyInputManager();
    inputManager = NULL;

    //-- Closing network system
    networkManager->logout();
    networkManager->stop();
    RdNetworkManager::destroyNetworkManager();
    networkManager = NULL;

    //-- Closing audio system:
    AudioManager::destroyAudioManager();
    audioManager = NULL;

    //-- Closing mental map:
    RdMentalMap::destroyMentalMap();
    mentalMap = NULL;

    //-- Close img related ports:
    imageManager->stop();
    RdImageManager::destroyImageManager();
    imageManager = NULL;

    //-- Close robot:
    robotManager->disconnect();

    //-- Delete FSM:
    delete gameFSM;
    gameFSM = NULL;

    cleanupSDL;
    return true;
}

