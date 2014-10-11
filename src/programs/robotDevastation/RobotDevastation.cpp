// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RobotDevastation.hpp"

bool rd::RobotDevastation::configure(yarp::os::ResourceFinder &rf)
{
    //-- Show help
    //printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("RobotDevastation options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--id integer\n");
        printf("\t--name string\n");
        printf("\t--team integer\n");
        // Do not exit: let last layer exit so we get help from the complete chain.
    }
    printf("RobotDevastation using no additional special options.\n");

    //-- Get player data
    //-----------------------------------------------------------------------------
    rdRoot = ::getenv ("RD_ROOT");
    if (rdRoot!=NULL)
    {
        RD_INFO("The RD_ROOT is: %s\n",rdRoot);
    }
    else
    {
        RD_WARNING("No RD_ROOT environment variable!\n");
        rdRoot="../..";  //-- Allow to run from build/bin
    }

    if(rf.check("id"))
    {
        RD_INFO("id: %d\n",rf.find("id").asInt());
    }
    else
    {
        RD_ERROR("No id!\n");
        return false;
    }

    if(rf.check("name"))
    {
        RD_INFO("name: %s\n",rf.find("name").asString().c_str());
    }
    else
    {
        RD_ERROR("No name!\n");
        return false;
    }

    if(rf.check("team"))
    {
        RD_INFO("team: %d\n",rf.find("team").asInt());
    }
    else
    {
        RD_ERROR("No team!\n");
        return false;
    }

    //-- Init mentalMap
    mentalMap = RdMentalMap::getMentalMap();
    mentalMap->configure( rf.find("id").asInt() );

    std::vector< RdPlayer > players;
    players.push_back( RdPlayer(rf.find("id").asInt(),std::string(rf.find("name").asString()),100,100,rf.find("team").asInt(),0) );
    mentalMap->updatePlayers(players);

    mentalMap->addWeapon(RdWeapon("Default gun", 10, 5));

    //-- Init input manager
    inputManager = RdInputManager::getInputManager();
    inputManager->addInputEventListener(this);
    if (!inputManager->start() )
    {
        RD_ERROR("Could not init inputManager!\n");
        return false;
    }

    //-- Init sound
    if( ! initSound() )
        return false;

    audioManager->playMusic("bso", -1);

    //-- Init output thread
    rateThreadOutput.setRdRoot(rdRoot);
    rateThreadOutput.setInImg(&inImg);
    rateThreadOutput.init(rf);

    //-- Init process thread
    rateThreadProcess.setInImg(&inImg);
    rateThreadProcess.init(rf);

    //-- Init network manager
    networkManager = RdYarpNetworkManager::getNetworkManager();
    networkManager->addNetworkEventListener(mentalMap);
    mentalMap->addMentalMapEventListener((RdYarpNetworkManager *)networkManager);
    networkManager->login(mentalMap->getMyself());

    //-- Init robot
    robotManager = new RdRd1RobotManager();
    if( ! robotManager->connect() )
        return false;

    //-----------------OPEN REMAINING LOCAL PORTS------------//
    /// \todo Encapsulate this
    std::ostringstream s;
    s << "/";
    s << mentalMap->getMyself().getId();
    s << "/img:i";
    inImg.open(s.str().c_str());

    return true;
}

bool rd::RobotDevastation::onKeyUp(rd::RdKey k)
{
if (k.isPrintable() )
    {
        RD_SUCCESS( "Key \"%c\" was pressed!\n", k.getChar() );

        if ( k.getChar() == 'w')
        {
            robotManager->stopMovement();
        }
        else if ( k.getChar() == 'a')
        {
            robotManager->stopMovement();
        }
        else if ( k.getChar() == 's')
        {
            robotManager->stopMovement();
        }
        else if ( k.getChar() == 'd')
        {
            robotManager->stopMovement();
        }
    }
}

bool rd::RobotDevastation::onKeyDown(rd::RdKey k)
{
    if ( k.isControlKey() )
    {
        RD_SUCCESS( "Control key with code %d pressed!\n", k.getValue() );

        if ( k.getValue() == RdKey::KEY_SPACE)
        {
            //-- Do things to shoot
            mentalMap->shoot();
            RD_SUCCESS("Shoot!\n");
        }
        else if ( k.getValue() == RdKey::KEY_ESCAPE)
        {
            RD_SUCCESS("Exit!\n");
            this->interruptModule();
        }
    }
    else if (k.isPrintable() )
    {
        RD_SUCCESS( "Key \"%c\" was pressed!\n", k.getChar() );

        if ( k.getChar() == 'r')
        {
            RD_SUCCESS("Reload!\n");
            mentalMap->reload();
        }
        else if ( k.getChar() == 'q')
        {
            RD_SUCCESS("Exit!\n");
            this->interruptModule();
        }
        else if ( k.getChar() == 'w')
        {
            robotManager->moveForward();
        }
        else if ( k.getChar() == 'a')
        {
            robotManager->turnLeft();
        }
        else if ( k.getChar() == 'd')
        {
            robotManager->turnRight();
        }
        else if ( k.getChar() == 's')
        {
            robotManager->moveBackwards();
        }
    }
}

double rd::RobotDevastation::getPeriod()
{
    return 2.0;  // Fixed, in seconds, the slow thread that calls updateModule below
}

bool rd::RobotDevastation::updateModule()
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

bool rd::RobotDevastation::initSound()
{
    std::string rdRootStr(rdRoot);

    audioManager = RdAudioManager::getAudioManager();

    if ( ! audioManager->load(rdRootStr+"/share/sounds/RobotDevastationBSO.mp3", "bso", 0) )
        return false;

    if ( ! audioManager->load(rdRootStr+"/share/sounds/01_milshot.wav", "shoot", 1) )
        return false;

    if ( ! audioManager->load(rdRootStr+"/share/sounds/15_explosion.wav", "explosion", 1) )
        return false;

    if ( ! audioManager->load(rdRootStr+"/share/sounds/03_clip.wav", "noAmmo", 1) )
        return false;

    if ( ! audioManager->load(rdRootStr+"/share/sounds/04_milaction.wav", "reload", 1) )
        return false;

    return true;
}

bool rd::RobotDevastation::interruptModule()
{
    RD_INFO("Closing program...\n");

    rateThreadOutput.stop();
    rateThreadProcess.stop();

    //-- Detach listeners to avoid segmentation faults
    inputManager->removeInputEventListeners();
    networkManager->removeNetworkEventListeners();
    mentalMap->removeMentalMapEventListeners();

    //-- Closing input manager:
    RdInputManager::destroyInputManager();
    inputManager = NULL;

    //-- Closing network system
    networkManager->logout(mentalMap->getMyself());
    RdYarpNetworkManager::destroyNetworkManager();
    networkManager = NULL;

    //-- Closing audio system:
    RdAudioManager::destroyAudioManager();
    audioManager = NULL;

    //-- Closing mental map:
    RdMentalMap::destroyMentalMap();
    mentalMap = NULL;



    //-- Close img related ports:
    inImg.interrupt();
    inImg.close();

    robotManager->disconnect();

    return true;
}

