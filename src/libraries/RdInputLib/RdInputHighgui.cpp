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
    char currentInputKey = *inputKey;

    //-- Release the mutex
    pthread_mutex_unlock( inputKeyMutex);

    //-- Look for the key in the table and execute action(s):
    //! \todo Look for the key in the table and execute action(s)
    std::cout << "[Info] Received key: \"" << currentInputKey << "\"" << std::endl;

    if ( currentInputKey != -1 )
    {
        if (currentInputKey == ' ')   // SPACE: 1048608 (0x100020), LSB: 32 (' ')
        {
            std::cout << "[info] RdInputHighgui: The space bar was pressed." << std::endl;
            rdManagerBasePtr->shoot();
        }
        else if ( currentInputKey == 't' ) // Letter T
        {
            std::cout << "[info] RdInputHighgui: The key 't' was pressed." << std::endl;
            //-- This enables/disables the head tracking thing
            bool (* toggleHeadTrack) (void *);
            toggleHeadTrack = (bool (*)(void*)) rdManagerBasePtr->getFunctionByName("toggleHeadTrack");
            (*toggleHeadTrack)((void *) rdManagerBasePtr);
        }
        else if ( currentInputKey == '\x1b' ) // ESC: 1048603 (0x10001b), LSB: 27 ('\x1b')
        {
            std::cout << "[info] RdInputHighgui: The escape key was pressed. Bye!" << std::endl;
            rdManagerBasePtr->quit();
        }
        else if ( currentInputKey==99 )  // CTRL-C
        {
            std::cout << "[info] RdInputHighgui: CTRL-C key was pressed. Bye!" << std::endl;
            rdManagerBasePtr->quit();
        }
}
