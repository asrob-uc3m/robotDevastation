#include "RdInputHighgui.hpp"

bool rdlib::RdInputHighgui::setup()
{
    RD_INFO("RdInputHighgui: setup!\n");
    // alpha = list(string.ascii_lowercase)
    // for letter in alpha:
    //     print 'keyMap["'+letter+'"]=\''+letter+'\';'
    keyMap["a"]='a';
    keyMap["b"]='b';
    keyMap["c"]='c';
    keyMap["d"]='d';
    keyMap["e"]='e';
    keyMap["f"]='f';
    keyMap["g"]='g';
    keyMap["h"]='h';
    keyMap["i"]='i';
    keyMap["j"]='j';
    keyMap["k"]='k';
    keyMap["l"]='l';
    keyMap["m"]='m';
    keyMap["n"]='n';
    keyMap["o"]='o';
    keyMap["p"]='p';
    keyMap["q"]='q';
    keyMap["r"]='r';
    keyMap["s"]='s';
    keyMap["t"]='t';
    keyMap["u"]='u';
    keyMap["v"]='v';
    keyMap["w"]='w';
    keyMap["x"]='x';
    keyMap["y"]='y';
    keyMap["z"]='z';

    keyMap["SPACE"]=' ';
    keyMap["ESC"]='\x1b';
    //keyMap["CTRL_C"]='c';  // This is bad... maybe catch an exception?
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
        pthread_mutex_unlock(inputKeyMutex);

        //-- Look for the key in the table and execute action(s):
        //! \todo Look for the key in the table and execute action(s)
        //std::cout << "[Info] Received key: \"" << (int)currentInputKey << "\"" << std::endl;

        if ((currentInputKey != -1)&&(lastInputKey == -1))
        {
            RD_DEBUG("Function called: %s\n", keyFunctionMap[currentInputKey].c_str() );
            rdManagerBasePtr->callFunctionByName( keyFunctionMap[currentInputKey] );

        }
        lastInputKey = currentInputKey;
    }
}
