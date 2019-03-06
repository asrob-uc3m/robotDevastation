// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "AudioManager.hpp"

#include <ColorDebug.h>

//-- This is very important:
rd::AudioManager * rd::AudioManager::audioManagerInstance = NULL;
std::string rd::AudioManager::currentId = "";
std::map<std::string, rd::AudioManager *> rd::AudioManager::audioManagerRegistry = std::map<std::string, rd::AudioManager *>();

//-- C++ forces us to initialize constants here:
const int rd::AudioManager::MUSIC = 0;
const int rd::AudioManager::FX = 1;

rd::AudioManager *rd::AudioManager::getAudioManager()
{
    if (audioManagerInstance == NULL )
    {
        if(!audioManagerRegistry.empty() )
        {
            //-- Install the first ImageManager found
            currentId = audioManagerRegistry.begin()->first;
            audioManagerInstance = audioManagerRegistry.begin()->second;
        }
        else
        {
            CD_ERROR("No AudioManager registered!! Returning null\n");
        }
    }

    return audioManagerInstance;
}

rd::AudioManager *rd::AudioManager::getAudioManager(const std::string & id)
{
    if (audioManagerInstance == NULL )
    {
        if( audioManagerRegistry.find(id) != audioManagerRegistry.end() )
        {
            currentId = id;
            audioManagerInstance = audioManagerRegistry[id];
        }
        else
        {
            CD_ERROR("AudioManager %s not found! (Maybe it was not registered?)\n", id.c_str());
        }
    }

    return audioManagerInstance;
}

bool rd::AudioManager::destroyAudioManager()
{
    //-- First, stop & delete the manager currently in use (if any)
    if (audioManagerInstance != NULL)
    {
        audioManagerInstance->stop();

        delete audioManagerInstance;
        audioManagerInstance = NULL;

        //-- Remove it also from the registry
        std::map<std::string, AudioManager *>::iterator it = audioManagerRegistry.find(currentId);
        if (it != audioManagerRegistry.end())
            audioManagerRegistry.erase(it);
    }

    //-- Destroy all the remaining registered InputManagers
    for ( std::map<std::string, AudioManager *>::iterator it = audioManagerRegistry.begin();
          it != audioManagerRegistry.end(); ++it)
    {
        AudioManager * currentManager = it->second;

        if (currentManager != NULL)
        {
            currentManager->stop();

            delete currentManager;
            currentManager = NULL;
         }

         //-- Remove it also from the registry
         audioManagerRegistry.erase(it);
    }

    return true;
}

rd::AudioManager::~AudioManager()
{
}

bool rd::AudioManager::configure(const std::string & parameter, const std::string & value)
{
    return true;
}

bool rd::AudioManager::Register(AudioManager *manager, const std::string & id)
{
    if ( audioManagerRegistry.find(id) == audioManagerRegistry.end())
    {
        if (manager != NULL)
        {
            audioManagerRegistry[id] = manager;
            return true;
        }
        else
        {
            CD_ERROR("Trying to register a NULL manager with id \"%s\"\n", id.c_str());
            return false;
        }
    }
    else
    {
        CD_WARNING( "(known issue) AudioManager with id \"%s\" was already registered\n", id.c_str());  // Change to CD_ERRROR after fixing #30
        return true;  // Return false after fixing #30
    }

}
