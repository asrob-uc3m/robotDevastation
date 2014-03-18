#include "RdInputHighgui.hpp"

bool rdlib::RdInputHighgui::setup()
{
    RD_INFO("RdInputHighgui: setup!\n");
    keyMap["a"]='a';
    keyMap["d"]='d';
    keyMap["s"]='s';
    keyMap["w"]='w';
    keyMap["t"]='t';
    keyMap["SPACE"]=' ';
    keyMap["ESC"]='\x1b';
    keyMap["CTRL_C"]='c';  // This is bad... maybe catch?
}

bool rdlib::RdInputHighgui::start()
{
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
            bool (* functionPointer) (void *);
            functionPointer = (bool (*)(void*)) keyFunctionMap[currentInputKey];

            //-- Call the function through the pointer as long as it is not NULL
            if ( functionPointer )
                (*functionPointer)((void *) rdManagerBasePtr);

        }
        lastInputKey = currentInputKey;
    }
}
