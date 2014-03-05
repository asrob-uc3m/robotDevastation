#include "RdInputHighgui.hpp"

bool rdlib::RdInputHighgui::start()
{
    if ( rdOutputHighguiPtr != NULL && inputKeyMutex != NULL && inputKey != NULL )
    {
        rdlib::RdInputBase::start();
    }
    else
    {
        std::cerr << "[Error] RdInputHighgui: Could not start RdInputHighgui, required references not found." << std::endl;
    }
}

bool rdlib::RdInputHighgui::setRdOutputHighguiPtr(rdlib::RdOutputHighgui *rdOutputHighgui)
{
    this->rdOutputHighguiPtr = rdOutputHighguiPtr;
    if ( rdOutputHighguiPtr == NULL )
    {
        std::cerr << "[Error] RdInputHighgui: Error setting reference to Highgui output module." << std::endl;
        return false;
    }
    else
    {
        this->inputKeyMutex = rdOutputHighguiPtr->getInputKeyMutex();
        this->inputKey = rdOutputHighguiPtr->getInputKey();

    }
}

bool rdlib::RdInputHighgui::input()
{
    //-- Lock the mutex to access the key
    pthread_mutex_lock( inputKeyMutex );

    //-- Get a copy of the key
    char currentInputKey = &inputKey;

    //-- Release the mutex
    pthread_mutex_unlock( inputKeyMutex);

    //-- Look for the key in the table and execute action(s):
    //! \todo Look for the key in the table and execute action(s)

}
