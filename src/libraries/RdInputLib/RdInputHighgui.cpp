#include "RdInputHighgui.hpp"

bool rdlib::RdInputHighgui::start()
{
    std::pair< std::string, char > kb_esc("ESC", '\x1b');
    keyMap.insert(kb_esc);
    std::pair< std::string, char > kb_w("W", 'w');
    keyMap.insert(kb_w);

    if ( rdOutputHighguiPtr == NULL )
    {
        RD_ERROR("Could not start RdInputHighgui,\n");
        RD_ERROR("required reference to OutputHighgui not found.\n");
        return false;
    }

    if ( inputKeyMutex == NULL )
    {
        RD_ERROR("Could not start RdInputHighgui,\n");
        RD_ERROR("required reference to inputKey mutex not found.\n");
        return false;
    }

    if ( inputKey == NULL )
    {
        RD_ERROR("Could not start RdInputHighgui,\n");
        RD_ERROR("required reference to inputKey not found.\n");
        return false;
    }

    return rdlib::RdInputBase::start();
}

bool rdlib::RdInputHighgui::setRdOutputHighguiPtr(rdlib::RdOutputHighgui *rdOutputHighgui)
{
    this->rdOutputHighguiPtr = rdOutputHighgui;
    if ( rdOutputHighguiPtr == NULL )
    {
        RD_ERROR("Error setting reference to\n");
        RD_ERROR("Highgui output module.\n");
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
    while ( !stopThread )
    {
        //-- Lock the mutex to access the key
        pthread_mutex_lock( inputKeyMutex );

        //-- Get a copy of the key
        char currentInputKey = *inputKey;

        //-- Release the mutex
        pthread_mutex_unlock( inputKeyMutex);

        //-- Look for the key in the table and execute action(s):
        //! \todo Look for the key in the table and execute action(s)
        //std::cout << "[Info] Received key: \"" << (int)currentInputKey << "\"" << std::endl;

        if ((currentInputKey != -1)&&(lastInputKey == -1))
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
                rdManagerBasePtr->askToStop();
            }
            else if ( currentInputKey==99 )  // CTRL-C
            {
                std::cout << "[info] RdInputHighgui: CTRL-C key was pressed. Bye!" << std::endl;
                rdManagerBasePtr->askToStop();
            }
        }
        lastInputKey = currentInputKey;
    }
}
