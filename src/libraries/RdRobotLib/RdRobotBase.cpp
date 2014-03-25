#include "RdRobotBase.hpp"

rdlib::RdRobotBase::RdRobotBase()
{
    RD_DEBUG("this: %p.\n",this);
    functionMap[ "reset" ] = (void *) &resetWrapper;
    functionMap[ "shoot" ] = (void *) &shootWrapper;
}

void *rdlib::RdRobotBase::getFunctionByName(std::string function_name)
{
    return functionMap[ function_name];
}

bool rdlib::RdRobotBase::shootWrapper(void *This)
{
    return (( rdlib::RdRobotBase *) This)->shoot();
}

bool rdlib::RdRobotBase::resetWrapper(void *This)
{
    return ((rdlib::RdRobotBase *) This)->quit();
}

void rdlib::RdRobotBase::setRdIniMap(std::map<std::string, std::string> &rdIniMap)
{
    this->rdIniMap = rdIniMap;
}

bool rdlib::RdRobotBase::callFunctionByName(const std::string& cmd)
{
    if( cmd == "shoot" )
    {
        shoot();
    }
}


