// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "SDLAudioManager.hpp"

#include <SDL.h>

#include <ColorDebug.hpp>

//-- This is very important:
rd::SDLAudioManager * rd::SDLAudioManager::uniqueInstance = NULL;
const std::string rd::SDLAudioManager::id = "SDL";

rd::SDLAudioManager::SDLAudioManager()
{
    if(SDL_Init(SDL_INIT_AUDIO)==-1)
        CD_ERROR("SDL Audio subsystem could not be initialized!\n");

    if (Mix_OpenAudio(22050, AUDIO_S16, 2, 4096) == -1)
        CD_ERROR("AudioMixer could not be opened: %s\n", Mix_GetError());

    stopped = true;
}


bool rd::SDLAudioManager::load(const std::string &music_filepath, const std::string &id, const int &type)
{
    if(type == MUSIC)
      {
        Mix_Music* pMusic = Mix_LoadMUS(music_filepath.c_str());

        if(pMusic == 0)
        {
            CD_ERROR( "Error loading file: \"%s\": %s\n", music_filepath.c_str(), Mix_GetError());
            return false;
        }

        music_sounds[id] = pMusic;
        return true;
      }
      else if(type == FX)
      {
        Mix_Chunk* pChunk = Mix_LoadWAV(music_filepath.c_str());
        if(pChunk == 0)
        {
            CD_ERROR( "Error loading file: \"%s\": %s\n", music_filepath.c_str(), Mix_GetError());
            return false;
        }

        fx_sounds[id] = pChunk;
        return true;
      }
    return false;
}

bool rd::SDLAudioManager::play(const std::string &id, int loop)
{
    if (music_sounds.find(id) != music_sounds.end())
    {
        //-- Play music
        if (Mix_PlayMusic(music_sounds[id], loop) == -1)
        {
            CD_ERROR( "Error playing music \"%s\"\n", id.c_str());
            return false;
        }
    }
    else if (fx_sounds.find(id) != fx_sounds.end())
    {
        //-- Play fx sound:
        if( Mix_PlayChannel(-1, fx_sounds[id], loop) == -1 )
        {
            CD_ERROR( "Error playing sound \"%s\"\n", id.c_str());
            return false;
        }
    }
    else
    {
        CD_ERROR( "Sound \"%s\" not found (maybe it was not loaded?)\n", id.c_str());
        return false;
    }

    return true;
}

bool rd::SDLAudioManager::stopMusic()
{
    if ( Mix_PlayingMusic() )
        if(Mix_HaltMusic() == -1)
        {
            CD_ERROR( "Error stopping music\n");
            return false;
        }

    return true;
}

bool rd::SDLAudioManager::start()
{
    stopped = false;
    return true;
}

bool rd::SDLAudioManager::stop()
{
    Mix_CloseAudio();
    stopped = true;
    return true;
}

bool rd::SDLAudioManager::isStopped() const
{
    return stopped;
}

bool rd::SDLAudioManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new SDLAudioManager();
    }

    return Register( uniqueInstance, id);
}

rd::SDLAudioManager::~SDLAudioManager()
{
    //-- Stop this manager
    stop();
    uniqueInstance = NULL;
}
