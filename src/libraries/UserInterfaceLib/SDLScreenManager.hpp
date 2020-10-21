// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_SDL_SCREEN_MANAGER_HPP__
#define __RD_SDL_SCREEN_MANAGER_HPP__

#include <string>
#include <vector>
#include <map>
#include <mutex>

#include <SDL.h>

#include "ScreenManager.hpp"
#include "Screen.hpp"
#include "ImageManager.hpp"
#include "Player.hpp"
#include "Target.hpp"
#include "Weapon.hpp"

namespace rd{

/**
 * @ingroup UserInterfaceLib
 *
 * @brief Manage game screens using SDL libraries
 *
 */
class SDLScreenManager : public ScreenManager
{
    public:
        //------------------------------- Screen Manager functions -----------------------------------------------------//    
        //! @brief Set a Screen as current Screen
        virtual void setCurrentScreen(Screen* screen);

        //! @brief Display the current Screen on the game window
        virtual bool show();

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(const std::string & parameter, const std::string & value);

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(const std::string & parameter, const Image & value); //-- Required by GameScreen and DeadScreen

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(const std::string & parameter, const Player & value); //-- Required by GameScreen

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(const std::string & parameter, const std::vector<Player> & value); //-- Required by GameScreen

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(const std::string & parameter, const std::vector<Target> & value); //-- Required by GameScreen

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(const std::string & parameter, const Weapon & value); //-- Required by GameScreen

        //! @brief SDL initialization
        static bool initSDL();

        //! @brief SDL Cleanup
        static bool cleanupSDL();

        //------------------------------ Configuration ----------------------------------------------------------------//
        //! @brief Configures a parameter with a value
        virtual bool configure(const std::string & parameter, const std::string & value);

        static const std::string PARAM_FULLSCREEN;

        //---------------- Manager Stuff ----------------------------------------------------------------------//
        virtual bool start();
        virtual bool stop();
        virtual bool isStopped() const;

        /**
         * @brief Register this manager in the ScreenManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance)
         */
        static bool RegisterManager();

        //! @brief Destructor. Used to reset the local static reference after destroying this manager
        virtual ~SDLScreenManager();


        //! @brief String that identifies this manager
        static const std::string id;


    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the RegisterManager() method
         */
        SDLScreenManager();

        SDLScreenManager(const SDLScreenManager &);
        SDLScreenManager & operator=(const SDLScreenManager &);

        //! \brief Stores the unique instance of the SDLScreenManager
        static SDLScreenManager * uniqueInstance;

        bool stopped;
        bool sdl_initialized;
        bool fullscreen;

        SDL_Window * window;
        mutable std::mutex mutex;
};

}

#endif //-- __RD_SDL_SCREEN_MANAGER_HPP__
