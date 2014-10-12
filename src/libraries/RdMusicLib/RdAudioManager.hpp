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
 * @brief The RdMusicLib library contains the \ref RdAudioManager class.
 */

/**
 * @ingroup RdMusicLib
 *
 * @brief A Music class.
 *
 */
class RdAudioManager
{
    public:
        static RdAudioManager * getAudioManager();
        static bool destroyAudioManager();

        ~RdAudioManager();

        static const int MUSIC;
        static const int FX;

        bool load( const std::string& music_filepath, const std::string& id, const int& type);
        bool playMusic(const std::string& id, int loop);
        bool playSound(const std::string& id, int loop);
        bool stopMusic();

    private:
        RdAudioManager();

        static RdAudioManager * audioManagerInstance;
        std::map<std::string, Mix_Chunk*> fx_sounds;
        std::map<std::string, Mix_Music*> music_sounds;

};

}

#endif //-- __RD_AUDIO_MANAGER_HPP__
