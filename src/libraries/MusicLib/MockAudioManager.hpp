// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __MOCK_AUDIO_MANAGER_HPP__
#define __MOCK_AUDIO_MANAGER_HPP__

#include <map>

#include "Macros.hpp"
#include "AudioManager.hpp"

#include <yarp/os/RateThread.h>
#include <yarp/os/Mutex.h>

namespace rd{

/**
 * @ingroup MusicLib
 *
 * @brief Mock music and sound effects manager for testing purposes
 *
 */
class MockAudioManager : public AudioManager,
                           public yarp::os::RateThread
{
    public:
        //---------------- Mock interface -------------------------------------------------------------------------//
         //! @brief Whether the sound with the given id is being played or not
        bool isPlaying(const std::string& id);


        //---------------- Audio-related Stuff ----------------------------------------------------------------------//
        //! @brief Loads an audio file, assigning it a string as identifier
        bool load( const std::string& music_filepath, const std::string& id, const int& type);

        //! @brief Plays a music/sound effect file previously loaded
        bool play(const std::string &id, int loop);

        //! @brief Stops the music being played currently
        bool stopMusic();


        //---------------- Manager Stuff ----------------------------------------------------------------------//
        virtual bool start();
        virtual bool stop();
        virtual bool isStopped();

        /**
         * @brief Register this manager in the ImageManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance)
         */
        static bool RegisterManager();

        //! @brief Destructor. Used to reset the local static reference after destroying this manager
        ~MockAudioManager();

        //! @brief String that identifies this manager
        static const std::string id;

        //! @brief Default duration (s)
        static const int AUDIO_DURATION = 30;

        //! @brief Thread update period (ms)
        static const int UPDATE_PERIOD = 100;

    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the getAudioManager() method
         */
        MockAudioManager();

        //! @brief Method called periodically from the RateThread class. It simply calls the update() method.
        void run();

        //! \brief Stores the unique instance of the AudioManager
        static MockAudioManager * uniqueInstance;

        //! \brief Dictionary for sound durations:
        yarp::os::Mutex durations_mutex;
        std::map<std::string, int> durations;

        //! \brief Dictionary for loop times
        yarp::os::Mutex loop_times_mutex;
        std::map<std::string, int> loop_times;

        bool stopped;
};

}

#endif //-- __MOCK_AUDIO_MANAGER_HPP__
