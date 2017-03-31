// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INPUT_MANAGER_HPP__
#define __RD_INPUT_MANAGER_HPP__

#include <string>
#include <vector>
#include <map>

#include "InputEventListener.hpp"

namespace rd{

/**
 * @ingroup rd_libraries
 *
 * \defgroup InputLib
 *
 * @brief The InputLib library contains classes related to user input (keyboard, mouse, etc).
 */

/**
 * @ingroup InputLib
 *
 * @brief User input manager ( keyboard, mouse, joysticks, etc)
 *
 * InputManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first get the reference to the InputManager with getInputManager() and then we
 * access the manager with that reference.
 *
 * When the program finishes, the InputManager can be deallocated using destroyInputManager().
 *
 * InputManager acts as a <a href="http://en.wikipedia.org/wiki/Facade_pattern">façade</a>, offering
 * a unified interface to the different managers for the mouse, keyboard, etc.
 *
 * Input events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the data relevant to the event triggered (i.e. what key was pressed)
 *
 */
class InputManager
{
    public:
        //------------------------------ Construction & destruction ---------------------------------------------------//
        /**
         * @brief Get a reference to the InputManager
         * @return By default, if no id is specified, this will return a reference to the first
         * InputManager that it can find in the registry, or NULL if no InputManager was registered.
         */
        static InputManager * getInputManager();

        /**
         * @brief Get a reference to the InputManager
         * @return The InputManager registered with the given id, NULL if the id is not found in
         * the registry.
         */
        static InputManager * getInputManager(std::string id);

        //! @brief Deallocate all the registered InputManager
        static bool destroyInputManager();

        virtual ~InputManager();


        //------------------------------ Manager Startup & Halting ----------------------------------------------------//
        /**
         * @brief Start to capture input events
         *
         * This function is supposed to be called after InputManager configuration.
         */
        virtual bool start() = 0;

        //! @brief Stop capturing input events
        virtual bool stop() = 0;

        //! @brief True if the manager is not active
        virtual bool isStopped() const = 0;

        //------------------------------ Configuration & Listeners ----------------------------------------------------//
        //! @brief Adds a InputEventListener to the list of observers to be notified of events
        bool addInputEventListener( InputEventListener * listener );

        //! @brief Unregisters all the InputEventListener stored
        bool removeInputEventListeners();

        //! @brief Configures a parameter with a value
        virtual bool configure(std::string parameter, std::string value);

        //------------------------------ Input Manager API ------------------------------------------------------------//
        //! @brief Get system info about input events
        virtual bool refreshEvents();

    protected:
        /**
         * @brief This function allows subclasses to install their unique instances in the singleton register to be
         * selected later by the user
         */
        static bool Register( InputManager * manager, std::string id);

        //! @brief Observers registered to be notified of input events
        std::vector<InputEventListener *> listeners;

    private:
        //! @brief Stores the unique instance of the InputManager
        static InputManager * inputManagerInstance;

        //! \brief Stores the id of the current unique instance used
        static std::string currentId;

        //! \brief Stores all the InputManager that have been registered
        static std::map< std::string, InputManager * > inputManagerRegistry;

};

}

#endif //-- __RD_INPUT_MANAGER_HPP__
