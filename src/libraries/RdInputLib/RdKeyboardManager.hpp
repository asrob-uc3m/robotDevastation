// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_KEYBOARD_MANAGER_HPP__
#define __RD_KEYBOARD_MANAGER_HPP__

#include "RdUtils.hpp"
#include "RdInputEventListener.hpp"
#include "RdKey.hpp"
#include "RdSDLKey.hpp"

#include <vector>

#include <yarp/os/RateThread.h>
#include "SDL/SDL.h"

namespace rd{

/**
 * @ingroup RdInputLib
 *
 * @brief Submodule in charge of the keyboard input
 *
 * RdKeyboardManager runs on its own thread using YARP's RateThread. The update function is called
 * every UPDATE_RATE_MS milliseconds to check for SDL keyboard events.
 *
 * Input events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the data relevant to the event triggered (i.e. what key was pressed).
 */
class RdKeyboardManager : public yarp::os::RateThread
{
    public:
        RdKeyboardManager();
        ~RdKeyboardManager();

        //! @brief Adds a reference to a list of observers to be notified of events
        bool setInputEventListeners( std::vector<RdInputEventListener *> * listeners );

        //-- Input thread related functions:
        //! @brief Start to capture keyboard input events
        bool start();
        //! @brief Stop the submodule
        bool stop();

    private:
        //! @brief Method that checks for keyboard input events and notifies the listeners when a event occurs.
        bool update();
        //! @brief Method called periodically from the RateThread class. It simply calls the update() method.
        void run();

        //! @brief Period in milliseconds between keyboard event updates
        static const int UPDATE_RATE_MS;

        //! @brief Reference to a list of observers to be notified of the keyboard events
        std::vector<RdInputEventListener *> * listeners;

};

}

#endif //-- __RD_KEYBOARD_MANAGER_HPP__
