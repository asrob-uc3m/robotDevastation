// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SDL_INPUT_MANAGER_HPP__
#define __RD_SDL_INPUT_MANAGER_HPP__

#include <SDL/SDL.h>
#include <X11/Xlib.h>
#include <yarp/os/RateThread.h>

#include "RdInputManager.hpp"
#include "RdInputEventListener.hpp"
#include "RdKey.hpp"
#include "RdSDLKey.hpp"

namespace rd{

/**
 * @ingroup RdInputLib
 *
 * @brief User input manager ( keyboard, mouse, joysticks, etc) using SDL
 *
 * RdSDLInputManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first get the reference to the RdInputManager with getInputManager() and then we
 * access the manager with that reference.
 *
 * When the program finishes, the RdSDLInputManager can be deallocated using destroyInputManager().
 *
 * RdSDLInputManager acts as a <a href="http://en.wikipedia.org/wiki/Facade_pattern">façade</a>, offering
 * a unified interface to the different managers for the mouse, keyboard, etc.
 *
 * Input events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the data relevant to the event triggered (i.e. what key was pressed)
 *
 */
class RdSDLInputManager : public RdInputManager,
                          public yarp::os::RateThread
{
    public:
        virtual bool start();
        virtual bool stop();
        virtual bool isStopped();

        virtual bool configure(std::string parameter, std::string value);

        /**
         * @brief Register this manager in the RdImageManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance)
         */
        static bool RegisterManager();

        //! @brief Destructor. Used to reset the local static reference after destroying this manager
        ~RdSDLInputManager();

        //! @brief String that identifies this manager
        static const std::string id;

    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the getInputManager() method
         */
        RdSDLInputManager();

        //! @brief Method that checks for input events and notifies the listeners when a event occurs.
        bool update();

        //! @brief Method called periodically from the RateThread class. It simply calls the update() method.
        void run();

        //! @brief Reference to this manager (unique instance)
        static RdSDLInputManager * uniqueInstance;

        //! @brief Period in milliseconds between keyboard event updates
        static const int UPDATE_RATE_MS;
};
}

#endif // __RD_SDL_INPUT_MANAGER_HPP__
