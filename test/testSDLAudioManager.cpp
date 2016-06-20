#include "gtest/gtest.h"
#include <string>
#include "yarp/os/ResourceFinder.h"

#include "SDLAudioManager.hpp"

using namespace rd;

class MockupAudioManagerTest : public testing::Test
{
	public:
        virtual void SetUp()
        {
            SDLAudioManager::RegisterManager();
            audioManager = AudioManager::getAudioManager("SDL");

            //-- Find resources
            yarp::os::ResourceFinder rf;
            rf.setDefaultContext("robotDevastation");
            rf.setDefaultConfigFile("robotDevastation.ini");
            sound_bso_realpath = rf.findFileByName(sound_bso);
            sound_shoot_realpath = rf.findFileByName(sound_shoot);
            sound_explosion_realpath = rf.findFileByName(sound_explosion);

        }

        virtual void TearDown()
        {
           AudioManager::destroyAudioManager();
        }


	protected:
        AudioManager * audioManager;
        static const std::string sound_bso;
        static const std::string sound_shoot;
        static const std::string sound_explosion;

        std::string sound_bso_realpath;
        std::string sound_shoot_realpath;
        std::string sound_explosion_realpath;
};


const std::string MockupAudioManagerTest::sound_bso = "../sounds/RobotDevastationBSO.mp3";
const std::string MockupAudioManagerTest::sound_shoot = "../sounds/01_milshot.wav";
const std::string MockupAudioManagerTest::sound_explosion = "../sounds/15_explosion.wav";


TEST_F( MockupAudioManagerTest, AudioManagerIsSingleton)
{
    AudioManager * manager2 = NULL;
    manager2 = AudioManager::getAudioManager("SDL");

    ASSERT_NE((AudioManager *)NULL, audioManager);
    ASSERT_NE((AudioManager *)NULL, manager2);
    ASSERT_EQ(audioManager, manager2);
}

TEST_F( MockupAudioManagerTest, AudioManagerLoadsAudio)
{
    ASSERT_NE((AudioManager *)NULL, audioManager);
    ASSERT_TRUE(audioManager->load(sound_bso_realpath, "bso", SDLAudioManager::MUSIC));
    ASSERT_TRUE(audioManager->load(sound_shoot_realpath, "shoot", SDLAudioManager::FX));
    ASSERT_TRUE(audioManager->load(sound_explosion_realpath, "explosion", SDLAudioManager::FX));
}

TEST_F( MockupAudioManagerTest, AudioManagerPlaysOneSound )
{
    ASSERT_NE((AudioManager *)NULL, audioManager);

    ASSERT_TRUE(audioManager->load(sound_bso_realpath, "bso", SDLAudioManager::MUSIC));

    EXPECT_TRUE(audioManager->play("bso", true));
    sleep(2);
    EXPECT_TRUE(audioManager->stopMusic());
}

TEST_F( MockupAudioManagerTest, AudioManagerPlaysFx )
{
    ASSERT_NE((AudioManager *)NULL, audioManager);

    ASSERT_TRUE(audioManager->load(sound_shoot_realpath, "shoot", SDLAudioManager::FX));
    ASSERT_TRUE(audioManager->load(sound_explosion_realpath, "explosion", SDLAudioManager::FX));

    EXPECT_TRUE(audioManager->play("shoot", false));
    sleep(1);
    EXPECT_TRUE(audioManager->play("explosion", false));
    sleep(2);
}

TEST_F( MockupAudioManagerTest, AudioManagerPlaysAllSounds )
{
    ASSERT_NE((AudioManager *)NULL, audioManager);

    ASSERT_TRUE(audioManager->load(sound_bso_realpath, "bso", SDLAudioManager::MUSIC));
    ASSERT_TRUE(audioManager->load(sound_shoot_realpath, "shoot", SDLAudioManager::FX));
    ASSERT_TRUE(audioManager->load(sound_explosion_realpath, "explosion", SDLAudioManager::FX));

    EXPECT_TRUE(audioManager->play("bso", true));
    EXPECT_TRUE(audioManager->play("shoot", true));
    sleep(1);
    EXPECT_TRUE(audioManager->play("explosion", false));
    sleep(4);
    EXPECT_TRUE(audioManager->stopMusic());
}
