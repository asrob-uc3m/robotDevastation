#include "MockupAudioManager.hpp"

//-- This is very important:
rd::MockupAudioManager * rd::MockupAudioManager::uniqueInstance = NULL;
const std::string rd::MockupAudioManager::id = "MOCKUP";


rd::MockupAudioManager::MockupAudioManager() : yarp::os::RateThread(UPDATE_PERIOD)
{
    stopped = true;
}

void rd::MockupAudioManager::run()
{
    durations_mutex.lock();
    loop_times_mutex.lock();

    for( std::map<std::string, int>::const_iterator it = durations.begin(); it != durations.end(); ++it)
    {
       if (it->second == 0)
       {
           std::string id = it->first;
           if (loop_times[id] == -1)
           {
               durations[id] = AUDIO_DURATION;
           }
           else if (loop_times[id] > 0)
           {
               durations[id] = AUDIO_DURATION;
               loop_times[id]--;
           }
       }
       else if (it->second > 0)
       {
          durations[id]--;
       }
    }

    durations_mutex.unlock();
    loop_times_mutex.unlock();
}


bool rd::MockupAudioManager::isPlaying(const std::string &id)
{
    bool playing = false;

    durations_mutex.lock();
    loop_times_mutex.lock();

    if (durations.find(id) == durations.end() || loop_times.find(id) == loop_times.end())
    {
        RD_ERROR("Sound \"%s\" not found (maybe it was not loaded?)\n", id.c_str());
        durations_mutex.unlock();
        loop_times_mutex.unlock();
        return false;
    }

    if (loop_times[id] != 0 || durations[id] > 0)
    {
        playing = true;
    }

    durations_mutex.unlock();
    loop_times_mutex.unlock();

    return playing;
}

bool rd::MockupAudioManager::load(const std::string &music_filepath, const std::string &id, const int &type)
{
    durations_mutex.lock();
    loop_times_mutex.lock();

    if (durations.find(id) != durations.end() || loop_times.find(id) != loop_times.end())
    {
        RD_ERROR("Sound \"%s\" already exists\n", id.c_str());
        durations_mutex.unlock();
        loop_times_mutex.unlock();
        return false;
    }

    durations[id] = 0;
    loop_times[id] = 0;

    durations_mutex.unlock();
    loop_times_mutex.unlock();

    return true;
}

bool rd::MockupAudioManager::play(const std::string &id, int loop)
{
    durations_mutex.lock();
    loop_times_mutex.lock();

    if (durations.find(id) == durations.end() || loop_times.find(id) == loop_times.end())
    {
        RD_ERROR("Sound \"%s\" not found (maybe it was not loaded?)\n", id.c_str());
        durations_mutex.unlock();
        loop_times_mutex.unlock();
        return false;
    }

    durations[id] = AUDIO_DURATION;
    loop_times[id] = loop;

    durations_mutex.unlock();
    loop_times_mutex.unlock();

    return true;
}

bool rd::MockupAudioManager::stopMusic()
{
    durations_mutex.lock();
    loop_times_mutex.lock();

    for( std::map<std::string, int>::const_iterator it = durations.begin(); it != durations.end(); ++it)
    {
        std::string id = it->first;

        durations[id] = 0;
        loop_times[id]= 0;
    }

    durations_mutex.unlock();
    loop_times_mutex.unlock();

    return true;
}

bool rd::MockupAudioManager::start()
{
    stopped = false;
    return yarp::os::RateThread::start();
}

bool rd::MockupAudioManager::stop()
{
    durations_mutex.unlock();
    loop_times_mutex.unlock();
    yarp::os::RateThread::askToStop();
    stopped = true;
    return true;
}

bool rd::MockupAudioManager::isStopped()
{
    return stopped;
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
