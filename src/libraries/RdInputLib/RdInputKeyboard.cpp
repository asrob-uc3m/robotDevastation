// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdInputKeyboard.hpp"

namespace rdlib{

/************************************************************************/

void* RdInputKeyboard::staticKeyThreadFunction(void *arg) {
    return reinterpret_cast<RdInputKeyboard*>(arg)->keyThreadFunction(arg);
}

/************************************************************************/

void* RdInputKeyboard::keyThreadFunction(void *This) {
    std::cout << "[success] RdInputKeyboard thread." << std::endl;
    while (((RdInputKeyboard*)This)->isRunning) {
        XNextEvent(dis, &report);
        switch  (report.type) {
            case KeyPress:
                if (XLookupKeysym(&report.xkey, 0) == XK_space)  {
                    std::cout << "The space bar was pressed." << std::endl;
                    rdManagerBasePtr->shoot();
                } else if (XLookupKeysym(&report.xkey, 0) == XK_Escape) {
                    std::cout << "The escape key was pressed. Bye!" << std::endl;
                    isRunning = false;
                }
            default: 
                break;
        }
    }
}

/************************************************************************/

RdInputKeyboard::RdInputKeyboard() {
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

    int error = pthread_create (&threadId, NULL, &RdInputKeyboard::staticKeyThreadFunction, this);
    if (error == 0) {
        std::cout << "[success] RdInputKeyboard created thread." << std::endl;
    } else {
        std::cerr << "[warning] RdInputKeyboard could not create thread." << std::endl;
    }

}

/************************************************************************/

RdInputKeyboard::~RdInputKeyboard() {
    std::cout << "[info] RdInputKeyboard::~RdInputKeyboard()"<< std::endl;
}

/************************************************************************/

bool RdInputKeyboard::quit() {
    std::cout << "[info] RdInputKeyboard quit()" << std::endl;
    isRunning = false;
}

/************************************************************************/

} //rdlib

