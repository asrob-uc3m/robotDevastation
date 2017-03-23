// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INPUT_EVENT_LISTENER_HPP__
#define __RD_INPUT_EVENT_LISTENER_HPP__

#include "Key.hpp"
#include "WindowEvent.hpp"

namespace rd{

/**
 * @ingroup RdInputLib
 *
 * @brief Interface for objects that can be notified of input events by the RdInputManager
 *
 * RdInputEventListener follows the <a href="http://en.wikipedia.org/wiki/Observer_pattern">observer</a> design pattern.
 */
class InputEventListener
{
    public:
         /**
         * @brief This function will be called whenever a key is pressed on the keyboard
         * @param k Key that triggered the event
         */
        virtual bool onKeyDown( const Key & k ) = 0;

        /**
        * @brief This function will be called whenever a key is released on the keyboard
        * @param k Key that triggered the event
        */
        virtual bool onKeyUp( const Key & k ) = 0;

        /**
         * @brief This function will be called whenever a window event is raised
         * @param event Object that wraps the window event
         */
        virtual bool onWindowEvent( const WindowEvent & event ) = 0;
};

}

#endif //-- __RD_INPUT_EVENT_LISTENER_HPP__
