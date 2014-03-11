#include "RdInputBase.hpp"

bool rdlib::RdInputBase::start()
{
    isRunning = true;
    int res = pthread_create (&threadId, NULL, &RdInputBase::inputThread, this);
    if (res == 0) {
        RD_SUCCESS("RdInputBase created thread.\n");
    } else {
        RD_WARNING("RdInputBase could not create thread.\n");
    }
}

bool rdlib::RdInputBase::quit()
{
    std::cout << "[info] RdInputBase quit()" << std::endl;
    isRunning = false;
    pthread_join( threadId, NULL);
}

void rdlib::RdInputBase::setRdManagerBasePtr(rdlib::RdManagerBase *rdManagerBasePtr) {
    this->rdManagerBasePtr = rdManagerBasePtr;
}


void * rdlib::RdInputBase::inputThread(void *This)
{
    reinterpret_cast<RdInputBase*>(This)->input();
}
