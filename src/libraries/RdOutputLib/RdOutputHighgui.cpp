// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdOutputHighgui.hpp"

namespace rdlib{

/************************************************************************/

void* RdOutputHighgui::staticKeyThreadFunction(void *arg) {
    return reinterpret_cast<RdOutputHighgui*>(arg)->keyThreadFunction(arg);
}

/************************************************************************/

void* RdOutputHighgui::keyThreadFunction(void *This) {
    std::cout << "[success] RdOutputHighgui thread." << std::endl;
    while (((RdOutputHighgui*)This)->isRunning) {
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

RdOutputHighgui::RdOutputHighgui() {
    dis = XOpenDisplay(NULL);
    if (!dis) {
        std::cerr << "[warning] RdOutputHighgui unable to connect to display." << std::endl;
    } else {
        std::cout << "[success] RdOutputHighgui connected to display." << std::endl;
    }
    at.event_mask = KeyPressMask;  // needed?
    win = XCreateWindow( dis, RootWindow(dis, 0), 10,10,200,200, 0, CopyFromParent, InputOnly, 0, 0, &at );    

    /* tell the display server what kind of events we would like to see */
    XSelectInput(dis, win, ButtonPressMask|StructureNotifyMask|KeyPressMask|KeyReleaseMask|KeymapStateMask);

    XMapWindow(dis, win);
    XFlush(dis);
    isRunning=true;

    int error = pthread_create (&threadId, NULL, &RdOutputHighgui::staticKeyThreadFunction, this);
    if (error == 0) {
        std::cout << "[success] RdOutputHighgui created thread." << std::endl;
    } else {
        std::cerr << "[warning] RdOutputHighgui could not create thread." << std::endl;
    }

}

/************************************************************************/

RdOutputHighgui::~RdOutputHighgui() {
    std::cout << "[info] RdOutputHighgui::~RdOutputHighgui()"<< std::endl;
}

/************************************************************************/

bool RdOutputHighgui::stop() {
    std::cout << "[info] RdOutputHighgui stop()" << std::endl;
    isRunning = false;
}

/************************************************************************/

} //rdlib

