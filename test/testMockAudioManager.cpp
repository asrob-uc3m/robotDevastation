#include "gtest/gtest.h"
#include <string>

#include "MockAudioManager.hpp"

#include <yarp/os/Time.h>
#include <yarp/os/ResourceFinder.h>

using namespace rd;

class MockupAudioManagerTest : public testing::Test
{
	public:
        virtual void SetUp()
        {
            MockupAudioManager::RegisterManager();
            audioManager = AudioManager::getAudioManager("MOCKUP");
            mockupManager = (MockupAudioManager *) audioManager;

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
           mockupManager = NULL;
           audioManager = NULL;
        }


	protected:
        MockupAudioManager * mockupManager;
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
    manager2 = AudioManager::getAudioManager("MOCKUP");

    ASSERT_NE((AudioManager *)NULL, audioManager);
    ASSERT_NE((AudioManager *)NULL, manager2);
    ASSERT_EQ(audioManager, manager2);
}

TEST_F( MockupAudioManagerTest, AudioManagerLoadsAudio)
{
    ASSERT_NE((AudioManager *)NULL, audioManager);
    ASSERT_TRUE(audioManager->load(sound_bso_realpath, "bso", AudioManager::MUSIC));
    ASSERT_TRUE(audioManager->load(sound_shoot_realpath, "shoot", AudioManager::FX));
    ASSERT_TRUE(audioManager->load(sound_explosion_realpath, "explosion", AudioManager::FX));
}

TEST_F( MockupAudioManagerTest, AudioManagerPlaysOneSound )
{
    ASSERT_NE((AudioManager *)NULL, audioManager);

    ASSERT_TRUE(audioManager->load(sound_bso_realpath, "bso", AudioManager::MUSIC));
    ASSERT_TRUE(audioManager->start());

    EXPECT_TRUE(audioManager->play("bso", true));
    EXPECT_TRUE(mockupManager->isPlaying("bso"));
    yarp::os::Time::delay(2);
    EXPECT_TRUE(audioManager->stopMusic());
    EXPECT_FALSE(mockupManager->isPlaying("bso"));
    EXPECT_TRUE(audioManager->stop());
}

TEST_F( MockupAudioManagerTest, AudioManagerPlaysFx )
{
    ASSERT_NE((AudioManager *)NULL, audioManager);

    ASSERT_TRUE(audioManager->load(sound_shoot_realpath, "shoot", AudioManager::FX));
    ASSERT_TRUE(audioManager->load(sound_explosion_realpath, "explosion", AudioManager::FX));
    ASSERT_TRUE(audioManager->start());

    EXPECT_TRUE(audioManager->play("shoot", false));
    yarp::os::Time::delay(1);
    EXPECT_TRUE(audioManager->play("explosion", false));
    yarp::os::Time::delay(2);
    EXPECT_TRUE(audioManager->stop());
}

TEST_F( MockupAudioManagerTest, AudioManagerPlaysAllSounds )
{
    ASSERT_NE((AudioManager *)NULL, audioManager);

    ASSERT_TRUE(audioManager->load(sound_bso_realpath, "bso", AudioManager::MUSIC));
    ASSERT_TRUE(audioManager->load(sound_shoot_realpath, "shoot", AudioManager::FX));
    ASSERT_TRUE(audioManager->load(sound_explosion_realpath, "explosion", AudioManager::FX));
    ASSERT_TRUE(audioManager->start());

    EXPECT_TRUE(audioManager->play("bso", true));
    EXPECT_TRUE(audioManager->play("shoot", true));
    EXPECT_TRUE(mockupManager->isPlaying("bso"));
    EXPECT_TRUE(mockupManager->isPlaying("shoot"));
    yarp::os::Time::delay(1);
    EXPECT_TRUE(audioManager->play("explosion", false));
    EXPECT_TRUE(mockupManager->isPlaying("explosion"));
    yarp::os::Time::delay(4);

    EXPECT_TRUE(audioManager->stopMusic());
    EXPECT_FALSE(mockupManager->isPlaying("bso"));
    EXPECT_FALSE(mockupManager->isPlaying("shoot"));
    EXPECT_FALSE(mockupManager->isPlaying("explosion"));
    EXPECT_TRUE(audioManager->stop());
}
