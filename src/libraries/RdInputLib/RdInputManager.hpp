// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INPUT_MANAGER_HPP__
#define __RD_INPUT_MANAGER_HPP__

#include <SDL/SDL.h>

#include <vector>

#include "RdUtils.hpp"
#include "RdInputEventListener.hpp"
#include "RdKeyboardManager.hpp"


namespace rd{

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdInputManagerLib
 *
 * @brief The RdInputManagerLib library contains classes related to user input (keyboard, mouse, etc).
 */

/**
 * @ingroup RdInputLib
 *
 * @brief User input manager ( keyboard, mouse, joysticks, etc)
 *
 * RdInputManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first get the reference to the RdInputManager with getInputManager() and then we
 * access the manager with that reference.
 *
 * When the program finishes, the RdInputManager can be deallocated using destroyInputManager().
 *
 * RdInputManager acts as a <a href="http://en.wikipedia.org/wiki/Facade_pattern">façade</a>, offering
 * a unified interface to the different managers for the mouse, keyboard, etc.
 *
 * Input events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the data relevant to the event triggered (i.e. what key was pressed)
 *
 */
class RdInputManager
{
    public:
        //! @brief Get a reference to RdInputManager
        static RdInputManager * getInputManager();        
        //! @brief Deallocate the RdInputManager
        static bool destroyInputManager();

        ~RdInputManager();

        /**
         * @brief Start to capture input events
         *
         * This function is supposed to be called after RdInputManager configuration.
         */
        bool start();

        //! @brief Adds a RdInputEventListener to the list of observers to be notified of events
        bool addInputEventListener( RdInputEventListener * listener );
        //! @brief Unregisters all the RdInputEventListener stored
        bool removeInputEventListeners();

    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the getInputManager() method
         */
        RdInputManager();

        //! \brief Stores the unique instance of the RdInputManager
        static RdInputManager * inputManagerInstance;

        //! @brief Reference to the submodule in charge of keyboard events
        RdKeyboardManager * keyboardManager;

        //! @brief Observers registered to be notified of input events
        std::vector<RdInputEventListener *> listeners;

};

}

#endif //-- __RD_INPUT_MANAGER_HPP__
