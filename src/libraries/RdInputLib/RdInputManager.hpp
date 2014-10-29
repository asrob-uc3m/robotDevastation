// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INPUT_MANAGER_HPP__
#define __RD_INPUT_MANAGER_HPP__



#include <vector>

#include "RdUtils.hpp"
#include "RdInputEventListener.hpp"
#include "RdKeyboardManager.hpp"


namespace rd{

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdInputLib
 *
 * @brief The RdInputLib library contains classes related to user input (keyboard, mouse, etc).
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
        //------------------------------ Construction & destruction ---------------------------------------------------//
        /**
         * @brief Get a reference to the RdInputManager
         * @return By default, if no id is specified, this will return a reference to the first
         * RdInputManager that it can find in the registry, or NULL if no RdInputManager was registered.
         */
        static RdInputManager * getInputManager();

        /**
         * @brief Get a reference to the RdInputManager
         * @return The RdInputManager registered with the given id, NULL if the id is not found in
         * the registry.
         */
        static RdInputManager * getInputManager(std::string id);

        //! @brief Deallocate all the registered RdInputManager
        static bool destroyInputManager();

        virtual ~RdInputManager();


        //------------------------------ Manager Startup & Halting ----------------------------------------------------//
        /**
         * @brief Start to capture input events
         *
         * This function is supposed to be called after RdInputManager configuration.
         */
        bool start() = 0;

        //! @brief Stop capturing input events
        virtual bool stop() = 0;

        //------------------------------ Configuration & Listeners ----------------------------------------------------//
        //! @brief Adds a RdInputEventListener to the list of observers to be notified of events
        bool addInputEventListener( RdInputEventListener * listener );

        //! @brief Unregisters all the RdInputEventListener stored
        bool removeInputEventListeners();

        //! @brief Configures a parameter with a value
        virtual bool configure(std::string parameter, std::string value);

    protected:
        /**
         * @brief This function allows subclasses to install their unique instances in the singleton register to be
         * selected later by the user
         */
        static bool Register( RdInputManager * manager, std::string id);

        //! @brief Observers registered to be notified of input events
        std::vector<RdInputEventListener *> listeners;

    private:
        //! @brief Stores the unique instance of the RdInputManager
        static RdInputManager * inputManagerInstance;

        //! \brief Stores the id of the current unique instance used
        static std::string currentId;

        //! \brief Stores all the RdInputManager that have been registered
        static std::map< std::string, RdInputManager * > inputManagerRegistry;

};

}

#endif //-- __RD_INPUT_MANAGER_HPP__
