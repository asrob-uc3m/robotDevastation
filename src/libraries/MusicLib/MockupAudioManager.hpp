// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __MOCKUP_AUDIO_MANAGER_HPP__
#define __MOCKUP_AUDIO_MANAGER_HPP__

#include <map>

#include "RdMacros.hpp"
#include "AudioManager.hpp"

namespace rd{

/**
 * @ingroup RdMusicLib
 *
 * @brief Mockup music and sound effects manager for testing purposes
 *
 */
class MockupAudioManager : public AudioManager
{
    public:
        //---------------- Mockup interface -------------------------------------------------------------------------//
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

        /**
         * @brief Register this manager in the RdImageManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance)
         */
        static bool RegisterManager();

        //! @brief Destructor. Used to reset the local static reference after destroying this manager
        ~MockupAudioManager();


        //! @brief String that identifies this manager
        static const std::string id;


    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the getAudioManager() method
         */
        MockupAudioManager();

        //! \brief Stores the unique instance of the RdAudioManager
        static MockupAudioManager * uniqueInstance;

        //! \brief Dictionary for the sound effects
        std::map<std::string, bool> fx_sounds;

        //! \brief Dictionary for the music tracks
        std::map<std::string, bool> music_sounds;

};

}

#endif //-- __MOCKUP_AUDIO_MANAGER_HPP__
