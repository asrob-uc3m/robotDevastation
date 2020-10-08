// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_MOCK_AUDIO_MANAGER_HPP__
#define __RD_MOCK_AUDIO_MANAGER_HPP__

#include <string>
#include <map>
#include <mutex>

#include <yarp/os/PeriodicThread.h>

#include "AudioManager.hpp"

namespace rd{

/**
 * @ingroup MusicLib
 *
 * @brief Mock music and sound effects manager for testing purposes
 *
 */
class MockAudioManager : public AudioManager,
                         public yarp::os::PeriodicThread
{
    public:
        //---------------- Mock interface -------------------------------------------------------------------------//
         //! @brief Whether the sound with the given id is being played or not
        bool isPlaying(const std::string& id) const;


        //---------------- Audio-related Stuff ----------------------------------------------------------------------//
        bool load( const std::string& music_filepath, const std::string& id, const int& type);
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
        virtual ~MockAudioManager();

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
         * and the instantiation is done at the RegisterManager() method.
         */
        MockAudioManager();

        MockAudioManager(const MockAudioManager &);
        MockAudioManager & operator=(const MockAudioManager &);

        //! @brief Method called periodically from the PeriodicThread class. It simply calls the update() method.
        void run();

        //! \brief Stores the unique instance of the AudioManager
        static MockAudioManager * uniqueInstance;

        //! \brief Dictionary for sound durations:
        mutable std::mutex durations_mutex;
        std::map<std::string, int> durations;

        //! \brief Dictionary for loop times
        mutable std::mutex loop_times_mutex;
        std::map<std::string, int> loop_times;

        bool stopped;
};

}

#endif //-- __RD_MOCK_AUDIO_MANAGER_HPP__
