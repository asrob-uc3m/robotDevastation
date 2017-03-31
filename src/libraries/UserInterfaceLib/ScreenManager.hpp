// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SCREEN_MANAGER_HPP__
#define __RD_SCREEN_MANAGER_HPP__

#include <string>
#include <vector>
#include <map>

#include "Screen.hpp"
#include "ImageManager.hpp"
#include "Player.hpp"
#include "Target.hpp"
#include "Weapon.hpp"

namespace rd{

/**
 * @ingroup rd_libraries
 *
 * \defgroup UserInterfaceLib
 *
 * @brief The UserInterfaceLib library contains classes related to the game graphical user interface
 *
 **/

/**
 * @ingroup UserInterfaceLib
 *
 * @brief Manage game screens
 *
 * ScreenManager can load several audio files and assign them a string for identification,
 * so that they can be played later.
 *
 * ScreenManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first get the reference to the ScreenManager with getScreenManager() and then we
 * access the manager with that reference.
 *
 * When the program finishes, the ScreenManager can be deallocated using destroyScreenManager().
 */
class ScreenManager
{
    public:
        //------------------------------ Construction & destruction ---------------------------------------------------//
        /**
         * @brief Get a reference to the ScreenManager
         * @return By default, if no id is specified, this will return a reference to the first
         * ScreenManager that it can find in the registry, or NULL if no ScreenManager was registered.
         */
        static ScreenManager * getScreenManager();

        /**
         * @brief Get a reference to the ScreenManager
         * @return The ScreenManager registered with the given id, NULL if the id is not found in
         * the registry.
         */
        static ScreenManager * getScreenManager(std::string id);

        //! @brief Deallocate all the registered ScreenManager
        static bool destroyScreenManager();

        virtual ~ScreenManager();

        //------------------------------ Manager Startup & Halting ----------------------------------------------------//
        /**
         * @brief Start the ScreenManager
         *
         * This function is supposed to be called after ScreenManager configuration.
         */
        virtual bool start() = 0;

        //! @brief Stops the ScreenManager
        virtual bool stop() = 0;

        //! @brief True if the manager is not active
        virtual bool isStopped() const = 0;

        //------------------------------ Configuration ----------------------------------------------------------------//
        //! @brief Configures a parameter with a value
        virtual bool configure(std::string parameter, std::string value);


        //------------------------------- Screen Manager functions -----------------------------------------------------//
        //! @brief Set a Screen as current Screen
        virtual void setCurrentScreen(Screen* screen);

        //! @brief Display the current Screen on the game window
        virtual bool show() = 0;

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(std::string parameter, std::string value);

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(std::string parameter, Image value); //-- Required by GameScreen and DeadScreen

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(std::string parameter, Player value); //-- Required by GameScreen

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(std::string parameter, std::vector<Player> value); //-- Required by GameScreen

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(std::string parameter, std::vector<Target> value); //-- Required by GameScreen

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(std::string parameter, Weapon value); //-- Required by GameScreen

    protected:
        /**
         * @brief This function allows subclasses to install their unique instances in the singleton register to be
         * selected later by the user
         */
        static bool Register(ScreenManager * manager, std::string id);

        Screen * screen;

    private:
        //! @brief Stores the unique instance of the ScreenManager
        static ScreenManager * screenManagerInstance;

        //! \brief Stores the id of the current unique instance used
        static std::string currentId;

        //! \brief Stores all the ScreenManager that have been registered
        static std::map< std::string, ScreenManager * > screenManagerRegistry;



};

}

#endif //-- __RD_SCREEN_MANAGER_HPP__
