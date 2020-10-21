// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_SDL_INPUT_MANAGER_HPP__
#define __RD_SDL_INPUT_MANAGER_HPP__

#include <string>

#include <SDL.h>

#include "InputManager.hpp"

namespace rd{

/**
 * @ingroup InputLib
 *
 * @brief User input manager (keyboard, mouse, joysticks, etc) using SDL
 *
 * SDLInputManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first get the reference to the InputManager with getInputManager() and then we
 * access the manager with that reference.
 *
 * When the program finishes, the SDLInputManager can be deallocated using destroyInputManager().
 *
 * SDLInputManager acts as a <a href="http://en.wikipedia.org/wiki/Facade_pattern">façade</a>, offering
 * a unified interface to the different managers for the mouse, keyboard, etc.
 *
 * Input events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the data relevant to the event triggered (i.e. what key was pressed).
 *
 */
class SDLInputManager : public InputManager
{
    public:
        virtual bool start();
        virtual bool stop();
        virtual bool isStopped() const;

        virtual bool configure(const std::string & parameter, const std::string & value);

        /**
         * @brief Register this manager in the InputManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance).
         */
        static bool RegisterManager();

        //! @brief Destructor. Used to reset the local static reference after destroying this manager
        ~SDLInputManager();

        //! @brief String that identifies this manager
        static const std::string id;

        /**
         * @brief Get SDL info about input events
         *
         * In SDL, this function has to be called frequently, and in the same thread as the graphic output.
         */
        virtual bool refreshEvents();

    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the RegisterManager() method.
         */
        SDLInputManager();

        SDLInputManager(const SDLInputManager &);
        SDLInputManager & operator=(const SDLInputManager &);

        // This static function is the real callback function.  It's compatible
        // with the C-style CallbackFunctionPtr.  The extra void* is used to
        // get back into the real object of this class type.
        static int staticInputCallback(void *userdata, SDL_Event *event)
        {
            // Get back into the class by treating p as the "this" pointer.
            if( ! ((SDLInputManager *)userdata) -> inputCallback(event) )
                return 1;
            return 0;
        }

        bool inputCallback(SDL_Event* event);


        //! @brief Reference to this manager (unique instance)
        static SDLInputManager * uniqueInstance;

        bool stopped;
};
}

#endif // __RD_SDL_INPUT_MANAGER_HPP__
