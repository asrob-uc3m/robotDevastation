// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_AUDIO_MANAGER_HPP__
#define __RD_AUDIO_MANAGER_HPP__

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <iostream>
#include <map>

#include "RdMacros.hpp"

namespace rd{

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdMusicLib
 *
 * @brief The RdMusicLib library contains classes related to game music and sound effects

/**
 * @ingroup RdMusicLib
 *
 * @brief Music and sound effects manager
 *
 * RdAudioManger can load several audio files and assign them a string for identification,
 * so that they can be played later.
 *
 * RdAudioManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first get the reference to the RdAudioManager with getAudioManager() and then we
 * access the manager with that reference.
 *
 * When the program finishes, the RdAudioManager can be deallocated using destroAudioManager().
 */
class RdAudioManager
{
    public:
        //! @brief Get a reference to the RdAudioManager
        static RdAudioManager * getAudioManager();
        //! @brief Deallocate the RdAudioManager
        static bool destroyAudioManager();

        ~RdAudioManager();

        //! \brief Identifier for music tracks
        static const int MUSIC;
        //! \brief Identifier for sound effect tracks
        static const int FX;

        /**
         * @brief Loads an audio file, assigning it a string as identifier
         * @param music_filepath Path to the file to be loaded
         * @param id String that will identify the audio loaded
         * @param type Can take a value of MUSIC or FX depending if the audio is to be loaded as game music or
         * sound effect
         * @return True if sound was loaded successfully, false otherwise
         */
        bool load( const std::string& music_filepath, const std::string& id, const int& type);

        /**
         * @brief Plays a music file previously loaded
         * @param id String that identifies the music track
         * @param loop Number of times the music is played. -1 means play the music forever.
         * @return True if sound was played successfully, false otherwise
         */
        bool playMusic(const std::string& id, int loop);

        /**
         * @brief Plays a sound effect file previously loaded
         * @param id String that identifies the sound effect track
         * @param loop Number of times the effect is played. -1 means play the effect forever.
         * @return True if sound was played successfully, false otherwise
         */
        bool playSound(const std::string& id, int loop);

        /**
         * @brief Stops the music being played currently
         * @return False if the music could not be stopped
         */
        bool stopMusic();

    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the getAudioManager() method
         */
        RdAudioManager();

        //! \brief Stores the unique instance of the RdAudioManager
        static RdAudioManager * audioManagerInstance;

        //! \brief Dictionary for the sound effects
        std::map<std::string, Mix_Chunk*> fx_sounds;

        //! \brief Dictionary for the music tracks
        std::map<std::string, Mix_Music*> music_sounds;

};

}

#endif //-- __RD_AUDIO_MANAGER_HPP__
