#include "RdInputBase.hpp"

bool rdlib::RdInputBase::setup()
{
    //-- This does nothing (right now)
    RD_INFO("RdInputBase: setup!\n");
    return true;
}

bool rdlib::RdInputBase::start()
{
    stopThread = false;
    int result = pthread_create (&threadId, NULL, &RdInputBase::inputThread, this);
    if (result == 0) {
        RD_INFO("RdInputBase created thread.\n");
    }
    else
    {
        RD_WARNING("RdInputBase could not create thread.\n");
    }
}

bool rdlib::RdInputBase::askToStop()
{
    RD_INFO("RdInputBase: stopping...\n");
    stopThread = true;
    return true;
}

bool rdlib::RdInputBase::quit()
{
    RD_INFO("RdInputBase: exiting...\n");
    pthread_join( threadId, NULL);
}

void rdlib::RdInputBase::setRdManagerBasePtr(rdlib::RdManagerBase *rdManagerBasePtr) {
    this->rdManagerBasePtr = rdManagerBasePtr;
}


void * rdlib::RdInputBase::inputThread(void *This)
{
    reinterpret_cast<RdInputBase*>(This)->input();
}

const char& rdlib::RdInputBase::getKeyCharByName(std::string keychar_name)
{
    keyMap[ keychar_name ];
}

void rdlib::RdInputBase::setKeyFunctionMap(const std::map<char, std::string > &value)
{
    keyFunctionMap = value;
}
