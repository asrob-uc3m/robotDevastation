// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_KEYBOARD_MANAGER_HPP__
#define __RD_KEYBOARD_MANAGER_HPP__

#include "RdUtils.hpp"
#include "RdInputEventListener.hpp"

#include <vector>

#include <yarp/os/RateThread.h>
#include "SDL/SDL.h"

namespace rd{

class RdKeyboardManager : public yarp::os::RateThread
{
    public:
        RdKeyboardManager();
        ~RdKeyboardManager();

        bool setInputEventListeners( std::vector<RdInputEventListener *> * listeners );

        //-- Input thread related functions:

        bool start();
        bool stop();

    private:
        bool update();
        void run();

        static const int UPDATE_RATE_MS;
        std::vector<RdInputEventListener *> * listeners;

};

}

#endif //-- __RD_KEYBOARD_MANAGER_HPP__
