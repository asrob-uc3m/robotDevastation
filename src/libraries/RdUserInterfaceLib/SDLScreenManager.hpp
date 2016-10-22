// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __SDL_SCREEN_MANAGER_HPP__
#define __SDL_SCREEN_MANAGER_HPP__

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <map>

#include "RdMacros.hpp"
#include "ScreenManager.hpp"

namespace rd{

/**
 * @ingroup RdUserInterfaceLib
 *
 * @brief Manage game screens using SDL libraries
 *
 */
class SDLScreenManager : public ScreenManager
{
    public:
        //------------------------------- Screen Manager functions -----------------------------------------------------//
//        //! @brief Display the current RdScreen on the game window
//        virtual bool show();

//        //! @brief Update some RdScreen parameter through the ScreenManager
//        virtual bool update(std::string parameter, std::string value);

//        //! @brief Update some RdScreen parameter through the ScreenManager
//        virtual bool update(std::string parameter, RdImage value);

//        //! @brief Returns a reference to the game screen window (each screen will print directly on this window)
//        virtual void *getScreenWindow();

        //! @brief SDL initialization
        static bool initSDL();

        //! @brief SDL Cleanup
        static bool cleanupSDL();

        //---------------- Manager Stuff ----------------------------------------------------------------------//
        virtual bool start();
        virtual bool stop();
        virtual bool isStopped();

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

};

}

#endif //-- __SDL_SCREEN_MANAGER_HPP__
