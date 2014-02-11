#include "RdRobotBase.hpp"

rdlib::RdRobotBase::RdRobotBase()
{
    functionMap[ "reset" ] = &reset;
    functionMap[ "shoot" ] = &shoot;
}

void *rdlib::RdRobotBase::getFunctionByName(std::string function_name)
{
    return functionMap[ function_name];
}
