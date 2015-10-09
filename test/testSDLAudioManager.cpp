#include "gtest/gtest.h"
#include <string>

#include "SDLAudioManager.hpp"

using namespace rd;

class MockupAudioManagerTest : public testing::Test
{
	public:
        virtual void SetUp()
        {
            SDLAudioManager::RegisterManager();
            audioManager = AudioManager::getAudioManager("SDL");
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
};


const std::string MockupAudioManagerTest::sound_bso = "../../share/sounds/RobotDevastationBSO.mp3";
const std::string MockupAudioManagerTest::sound_shoot = "../../share/sounds/01_milshot.wav";
const std::string MockupAudioManagerTest::sound_explosion = "../../share/sounds/15_explosion.wav";


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
    ASSERT_TRUE(audioManager->load(sound_bso, "bso", SDLAudioManager::MUSIC));
    ASSERT_TRUE(audioManager->load(sound_shoot, "shoot", SDLAudioManager::FX));
    ASSERT_TRUE(audioManager->load(sound_explosion, "explosion", SDLAudioManager::FX));
}

TEST_F( MockupAudioManagerTest, AudioManagerPlaysOneSound )
{
    ASSERT_NE((AudioManager *)NULL, audioManager);

    ASSERT_TRUE(audioManager->load(sound_bso, "bso", SDLAudioManager::MUSIC));

    EXPECT_TRUE(audioManager->play("bso", true));
    sleep(2);
    EXPECT_TRUE(audioManager->stopMusic());
}

TEST_F( MockupAudioManagerTest, AudioManagerPlaysFx )
{
    ASSERT_NE((AudioManager *)NULL, audioManager);

    ASSERT_TRUE(audioManager->load(sound_shoot, "shoot", SDLAudioManager::FX));
    ASSERT_TRUE(audioManager->load(sound_explosion, "explosion", SDLAudioManager::FX));

    EXPECT_TRUE(audioManager->play("shoot", false));
    sleep(1);
    EXPECT_TRUE(audioManager->play("explosion", false));
    sleep(2);
}

TEST_F( MockupAudioManagerTest, AudioManagerPlaysAllSounds )
{
    ASSERT_NE((AudioManager *)NULL, audioManager);

    ASSERT_TRUE(audioManager->load(sound_bso, "bso", SDLAudioManager::MUSIC));
    ASSERT_TRUE(audioManager->load(sound_shoot, "shoot", SDLAudioManager::FX));
    ASSERT_TRUE(audioManager->load(sound_explosion, "explosion", SDLAudioManager::FX));

    EXPECT_TRUE(audioManager->play("bso", true));
    EXPECT_TRUE(audioManager->play("shoot", true));
    sleep(1);
    EXPECT_TRUE(audioManager->play("explosion", false));
    sleep(4);
    EXPECT_TRUE(audioManager->stopMusic());
}