// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_AUDIO_MANAGER_HPP__
#define __RD_AUDIO_MANAGER_HPP__

#include <string>
#include <map>

namespace rd{

/**
 * @ingroup MusicLib
 *
 * @brief Music and sound effects manager
 *
 * AudioManager can load several audio files and assign them a string for identification,
 * so that they can be played later.
 *
 * AudioManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first get the reference to the AudioManager with getAudioManager() and then we
 * access the manager with that reference.
 *
 * When the program finishes, the AudioManager can be deallocated using destroAudioManager().
 */
class AudioManager
{
    public:
        //------------------------------ Construction & destruction ---------------------------------------------------//
        /**
         * @brief Get a reference to the AudioManager
         * @return By default, if no id is specified, this will return a reference to the first
         * AudioManager that it can find in the registry, or NULL if no AudioManager was registered.
         */
        static AudioManager * getAudioManager();

        /**
         * @brief Get a reference to the AudioManager
         * @return The AudioManager registered with the given id, NULL if the id is not found in
         * the registry.
         */
        static AudioManager * getAudioManager(const std::string & id);

        //! @brief Deallocate all the registered AudioManager
        static bool destroyAudioManager();

        virtual ~AudioManager();


        //------------------------------ Manager Startup & Halting ----------------------------------------------------//
        /**
         * @brief Start the AudioManager
         *
         * This function is supposed to be called after AudioManager configuration.
         */
        virtual bool start() = 0;

        //! @brief Stops the AudioManager
        virtual bool stop() = 0;

        //! @brief True if the manager is not active
        virtual bool isStopped() const = 0;

        //------------------------------ Configuration ----------------------------------------------------------------//
        //! @brief Configures a parameter with a value
        virtual bool configure(const std::string & parameter, const std::string & value);


        //------------------------------- Audio Manager functions -----------------------------------------------------//
        /**
         * @brief Loads an audio file, assigning it a string as identifier
         * @param music_filepath Path to the file to be loaded
         * @param id String that will identify the audio loaded
         * @param type Can take a value of MUSIC or FX depending if the audio is to be loaded as game music or
         * sound effect
         * @return True if sound was loaded successfully, false otherwise
         */
        virtual bool load( const std::string& music_filepath, const std::string& id, const int& type) = 0;

        /**
         * @brief Plays a music/sound effect file previously loaded
         * @param id String that identifies the music / sound effect track
         * @param loop Number of times the music is played. -1 means play the music forever.
         * @return True if sound was played successfully, false otherwise
         */
        virtual bool play(const std::string& id, int loop = 1) = 0;

        /**
         * @brief Stops the music being played currently
         * @return False if the music could not be stopped
         */
        virtual bool stopMusic() = 0;

        //! \brief Identifier for music tracks
        static const int MUSIC;
        //! \brief Identifier for sound effect tracks
        static const int FX;

    protected:
        /**
         * @brief This function allows subclasses to install their unique instances in the singleton register to be
         * selected later by the user
         */
        static bool Register( AudioManager * manager, const std::string & id);


    private:
        //! @brief Stores the unique instance of the AudioManager
        static AudioManager * audioManagerInstance;

        //! \brief Stores the id of the current unique instance used
        static std::string currentId;

        //! \brief Stores all the AudioManager that have been registered
        static std::map< std::string, AudioManager * > audioManagerRegistry;

};

}

#endif //-- __RD_AUDIO_MANAGER_HPP__
