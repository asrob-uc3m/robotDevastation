// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdInputKeyboard.hpp"

rdlib::RdInputKeyboard::RdInputKeyboard()
{
    std::cout << "[info] RdInputKeyboard::RdInputKeyboard()" << std::endl;

    std::map< std::string, std::string > keyMap;
    RdIniReader< std::string, std::string > rdIniReader;
    rdIniReader.parseFile("../share/rdClient/conf/sample.ini", keyMap);

    dis = XOpenDisplay(NULL);
    if (!dis) {
        std::cerr << "[warning] RdInputKeyboard unable to connect to display." << std::endl;
    } else {
        std::cout << "[success] RdInputKeyboard connected to display." << std::endl;
    }
    at.event_mask = KeyPressMask;  // needed?
    win = XCreateWindow( dis, RootWindow(dis, 0), 10,10,200,200, 0, CopyFromParent, InputOnly, 0, 0, &at );

    /* tell the display server what kind of events we would like to see */
    XSelectInput(dis, win, ButtonPressMask|StructureNotifyMask|KeyPressMask|KeyReleaseMask|KeymapStateMask);

    XMapWindow(dis, win);
    XFlush(dis);
    isRunning=true;

    start(); //-- Note: Taking this out of here will give us more flexibility
}

bool rdlib::RdInputKeyboard::quit()
{
    std::cout << "[info] RdInputKeyboard quit()" << std::endl;
    isRunning = false;
    pthread_join( threadId, NULL);
}

//void* rdlib::RdInputKeyboard::keyThreadFunction(void *This)
bool rdlib::RdInputKeyboard::input()
{
    std::cout << "[success] RdInputKeyboard thread." << std::endl;
    while ( isRunning )
    {
        XNextEvent(dis, &report);
        switch  (report.type)
        {
            case KeyPress:
                if (XLookupKeysym(&report.xkey, 0) == XK_space)
                {
                    std::cout << "The space bar was pressed." << std::endl;
                    rdManagerBasePtr->shoot();
                }
                else if (XLookupKeysym(&report.xkey, 0) == XK_Escape)
                {
                    std::cout << "The escape key was pressed. Bye!" << std::endl;
                    rdManagerBasePtr->quit();
                }
            default: 
                break;
        }
    }
}

