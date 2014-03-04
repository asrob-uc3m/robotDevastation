#include "RdManagerBase.hpp"

rdlib::RdManagerBase::RdManagerBase()
{
    rdCameraBasePtr = 0;
    rdInputBasePtr = 0;
    rdOutputBasePtr = 0;
    rdRobotBasePtr = 0;

    //-- Add shoot to dictionary
    functionMap[ "shoot"] = (void *) &shootWrapper;
}

bool rdlib::RdManagerBase::shootWrapper(void *This)
{
    return (( rdlib::RdManagerBase *) This)->shoot();
}

void *rdlib::RdManagerBase::getFunctionByName( std::string function_name ) {
    return functionMap[ function_name ];
}

int rdlib::RdManagerBase::getManagerStatus() {
    return managerStatus;
}

void rdlib::RdManagerBase::setRdCameraBasePtr(RdCameraBase* rdCameraBasePtr ) {
    this->rdCameraBasePtr = rdCameraBasePtr;
}
void rdlib::RdManagerBase::setRdInputBasePtr(RdInputBase* rdInputBasePtr ) {
    this->rdInputBasePtr = rdInputBasePtr;
}
void rdlib::RdManagerBase::setRdOutputBasePtr(RdOutputBase* rdOutputBasePtr ) {
    this->rdOutputBasePtr = rdOutputBasePtr;
}
void rdlib::RdManagerBase::setRdRobotBasePtr(RdRobotBase* rdRobotBasePtr ) {
    this->rdRobotBasePtr = rdRobotBasePtr;
}

rdlib::RdCameraBase* rdlib::RdManagerBase::getRdCameraBasePtr() {
    return this->rdCameraBasePtr;
}

void rdlib::RdManagerBase::getEnemies( int index,  std::vector< std::pair<int, int> >& enemyPos,
                 std::vector< double >& enemySize) {
    enemyPos = this->enemyPos[index];
    enemySize = this->enemySize[index];
}
