#include "MockupAudioManager.hpp"

//-- This is very important:
rd::MockupAudioManager * rd::MockupAudioManager::uniqueInstance = NULL;
const std::string rd::MockupAudioManager::id = "MOCKUP";


rd::MockupAudioManager::MockupAudioManager()
{
}


bool rd::MockupAudioManager::isPlaying(const std::string &id)
{
    return false;
}

bool rd::MockupAudioManager::load(const std::string &music_filepath, const std::string &id, const int &type)
{
    return false;
}

bool rd::MockupAudioManager::play(const std::string &id, int loop)
{
    return false;
}

bool rd::MockupAudioManager::stopMusic()
{
    return false;
}

bool rd::MockupAudioManager::start()
{
    return false;
}

bool rd::MockupAudioManager::stop()
{
    return false;
}

bool rd::MockupAudioManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new MockupAudioManager();
    }

    return Register( uniqueInstance, id);
}

rd::MockupAudioManager::~MockupAudioManager()
{
    //-- Stop this manager
    this->stop();
    uniqueInstance = NULL;
}
