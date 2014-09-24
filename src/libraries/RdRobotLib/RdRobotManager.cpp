#include "RdRobotManager.hpp"

//-- This is very important:
rd::RdRobotManager * rd::RdRobotManager::robotManager = NULL;

std::vector<std::string> rd::RdRobotManager::listAll()
{
    std::vector<std::string> robot_manager_list;

    for( std::map<std::string, RdRobotManager*>::const_iterator it = robotManagerRegistry.begin(); it != robotManagerRegistry.end(); ++it)
    {
        robot_manager_list.push_back(it->first);
    }

    return robot_manager_list;
}

rd::RdRobotManager *rd::RdRobotManager::getRobotManager()
{
    //-- Work in progress
    return NULL;
}
