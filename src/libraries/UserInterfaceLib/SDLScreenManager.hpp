// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SDL_SCREEN_MANAGER_HPP__
#define __RD_SDL_SCREEN_MANAGER_HPP__

#include <string>
#include <vector>
#include <map>

#include <SDL.h>

#include <yarp/os/Mutex.h> //-- Right now use yarp mutex. In the future is better to use C++11 std::mutex

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
        virtual bool update(const std::string & parameter, Image value); //-- Required by GameScreen and DeadScreen

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(const std::string & parameter, Player value); //-- Required by GameScreen

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(const std::string & parameter, const std::vector<Player> & value); //-- Required by GameScreen

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(const std::string & parameter, const std::vector<Target> & value); //-- Required by GameScreen

        //! @brief Update some Screen parameter through the ScreenManager
        virtual bool update(const std::string & parameter, Weapon value); //-- Required by GameScreen

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
        ~SDLScreenManager();


        //! @brief String that identifies this manager
        static const std::string id;


    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the getScreenManager() method
         */
        SDLScreenManager();

        //! \brief Stores the unique instance of the SDLScreenManager
        static SDLScreenManager * uniqueInstance;

        bool stopped;
        bool sdl_initialized;
        bool fullscreen;

        SDL_Window * window;
        yarp::os::Mutex mutex;
};

}

#endif //-- __RD_SDL_SCREEN_MANAGER_HPP__
