
#include "RdAudioManager.hpp"


rd::RdAudioManager::RdAudioManager()
{
    if(SDL_Init(SDL_INIT_AUDIO)==-1)
        RD_ERROR("SDL Audio subsystem could not be initialized!");

    if(Mix_OpenAudio(22050, AUDIO_S16, 2, 4096)==-1)
        RD_ERROR("AudioMixer could not be opened!");
}

bool rd::RdAudioManager::load(const std::string &music_filepath, const std::string &id, const int &type)
{
    if(type == 0)
      {
        Mix_Music* pMusic = Mix_LoadMUS(music_filepath.c_str());

        if(pMusic == 0)
        {
            RD_ERROR( ("Error loading file: \"" + music_filepath + " " + std::string(Mix_GetError())).c_str() );
            return false;
        }

        music_sounds[id] = pMusic;
        return true;
      }
      else if(type == 1)
      {
        Mix_Chunk* pChunk = Mix_LoadWAV(music_filepath.c_str());
        if(pChunk == 0)
        {
            RD_ERROR( ("Error loading file: \"" + music_filepath + " " + std::string(Mix_GetError())).c_str() );
            return false;
        }

        fx_sounds[id] = pChunk;
        return true;
      }
      return false;
}

bool rd::RdAudioManager::playMusic(const std::string &id, int loop)
{
    if (Mix_PlayMusic(music_sounds[id], loop) == -1)
        return false;

    return true;
}

bool rd::RdAudioManager::stopMusic()
{
    if ( Mix_PlayingMusic() )
        Mix_HaltMusic();

    return true;
}
