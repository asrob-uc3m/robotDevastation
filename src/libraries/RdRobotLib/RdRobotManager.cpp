#include "RdRobotManager.hpp"

//-- This is very important:
rd::RdRobotManager * rd::RdRobotManager::robotManager = NULL;
std::map<std::string, rd::RdRobotManager *> rd::RdRobotManager::robotManagerRegistry = std::map<std::string, rd::RdRobotManager *>();


void rd::RdRobotManager::registerManager(std::string name, rd::RdRobotManager *manager)
{
    if ( robotManagerRegistry.find(name) == robotManagerRegistry.end() )
    {
        robotManagerRegistry[name] = manager;
    }
    else
    {
        RD_ERROR("RdManager with name \"%s\" already exists!\n", name.c_str());
    }
}

std::vector<std::string> rd::RdRobotManager::listAll()
{
    std::vector<std::string> robot_manager_list;

    for( std::map<std::string, RdRobotManager*>::const_iterator it = robotManagerRegistry.begin(); it != robotManagerRegistry.end(); ++it)
    {
        robot_manager_list.push_back(it->first);
    }

    return robot_manager_list;
}

rd::RdRobotManager *rd::RdRobotManager::getRobotManager(std::string name)
{
    if (robotManager == NULL)
    {
        if ( robotManagerRegistry.find(name) != robotManagerRegistry.end() )
        {
            robotManager = robotManagerRegistry[name];
        }
        else
        {
            RD_ERROR("RdRobotManager requested (\"%s\") does not exist.\n", name.c_str());
        }
    }

    return robotManager;
}

bool rd::RdRobotManager::destroyRobotManager()
{
    if (robotManager == NULL)
        return false;

    robotManager->onDestroy();
    delete robotManager;
    robotManager = NULL;

    return true;
}


rd::RdRobotManager::~RdRobotManager() {}
