#include "RdInputBase.hpp"

bool rdlib::RdInputBase::start()
{
    isRunning = true;
    int error = pthread_create (&threadId, NULL, &RdInputBase::inputThread, this);
    if (error == 0) {
        std::cout << "[success] RdInputKeyboard created thread." << std::endl;
    } else {
        std::cerr << "[warning] RdInputKeyboard could not create thread." << std::endl;
    }
}

bool rdlib::RdInputBase::quit()
{
    std::cout << "[info] RdInputKeyboard quit()" << std::endl;
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
