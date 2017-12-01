// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_INPUT_EVENT_LISTENER_HPP__
#define __RD_INPUT_EVENT_LISTENER_HPP__

#include "Key.hpp"
#include "WindowEvent.hpp"

namespace rd{

/**
 * @ingroup InputLib
 *
 * @brief Interface for objects that can be notified of input events by the InputManager
 *
 * InputEventListener follows the <a href="http://en.wikipedia.org/wiki/Observer_pattern">observer</a> design pattern.
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
         * @param event WindowEvent object that wraps the window event
         */
        virtual bool onWindowEvent( const WindowEvent & event ) = 0;

        virtual ~InputEventListener() {}
};

}

#endif //-- __RD_INPUT_EVENT_LISTENER_HPP__
