// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SDL_AUDIO_MANAGER_HPP__
#define __RD_SDL_AUDIO_MANAGER_HPP__

#include <map>
#include <string>

#include <SDL_mixer.h>

#include "AudioManager.hpp"

namespace rd{

/**
 * @ingroup MusicLib
 *
 * @brief Music and sound effects manager using SDL libraries
 *
 */
class SDLAudioManager : public AudioManager
{
    public:
        //---------------- Audio-related Stuff ----------------------------------------------------------------------//
        bool load(const std::string& music_filepath, const std::string& id, const int& type);
        bool play(const std::string &id, int loop);
        bool stopMusic();


        //---------------- Manager Stuff ----------------------------------------------------------------------//
        virtual bool start();
        virtual bool stop();
        virtual bool isStopped() const;

        /**
         * @brief Register this manager in the AudioManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance).
         */
        static bool RegisterManager();

        //! @brief Destructor. Used to reset the local static reference after destroying this manager
        virtual ~SDLAudioManager();


        //! @brief String that identifies this manager
        static const std::string id;


    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the RegisterManager() method
         */
        SDLAudioManager();

        SDLAudioManager(const SDLAudioManager &);
        SDLAudioManager & operator=(const SDLAudioManager &);

        //! \brief Stores the unique instance of the AudioManager
        static SDLAudioManager * uniqueInstance;

        //! \brief Dictionary for the sound effects
        std::map<std::string, Mix_Chunk*> fx_sounds;

        //! \brief Dictionary for the music tracks
        std::map<std::string, Mix_Music*> music_sounds;

        bool stopped;

};

}

#endif //-- __RD_SDL_AUDIO_MANAGER_HPP__
