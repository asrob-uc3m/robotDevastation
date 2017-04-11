#include "gtest/gtest.h"
#include <string>

#include "MockAudioManager.hpp"

#include <yarp/os/Time.h>
#include <yarp/os/ResourceFinder.h>

using namespace rd;

class MockAudioManagerTest : public testing::Test
{
	public:
        virtual void SetUp()
        {
            MockAudioManager::RegisterManager();
            audioManager = AudioManager::getAudioManager("MOCK");
            mockManager = (MockAudioManager *) audioManager;

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
           mockManager = NULL;
           audioManager = NULL;
        }


	protected:
        MockAudioManager * mockManager;
        AudioManager * audioManager;
        static const std::string sound_bso;
        static const std::string sound_shoot;
        static const std::string sound_explosion;

        std::string sound_bso_realpath;
        std::string sound_shoot_realpath;
        std::string sound_explosion_realpath;
};


const std::string MockAudioManagerTest::sound_bso = "../sounds/RobotDevastationBSO.mp3";
const std::string MockAudioManagerTest::sound_shoot = "../sounds/01_milshot.wav";
const std::string MockAudioManagerTest::sound_explosion = "../sounds/15_explosion.wav";


TEST_F( MockAudioManagerTest, AudioManagerIsSingleton)
{
    AudioManager * manager2 = NULL;
    manager2 = AudioManager::getAudioManager("MOCK");

    ASSERT_NE((AudioManager *)NULL, audioManager);
    ASSERT_NE((AudioManager *)NULL, manager2);
    ASSERT_EQ(audioManager, manager2);
}

TEST_F( MockAudioManagerTest, AudioManagerLoadsAudio)
{
    ASSERT_NE((AudioManager *)NULL, audioManager);
    ASSERT_TRUE(audioManager->load(sound_bso_realpath, "bso", AudioManager::MUSIC));
    ASSERT_TRUE(audioManager->load(sound_shoot_realpath, "shoot", AudioManager::FX));
    ASSERT_TRUE(audioManager->load(sound_explosion_realpath, "explosion", AudioManager::FX));
}

TEST_F( MockAudioManagerTest, AudioManagerPlaysOneSound )
{
    ASSERT_NE((AudioManager *)NULL, audioManager);

    ASSERT_TRUE(audioManager->load(sound_bso_realpath, "bso", AudioManager::MUSIC));
    ASSERT_TRUE(audioManager->start());

    EXPECT_TRUE(audioManager->play("bso", true));
    EXPECT_TRUE(mockManager->isPlaying("bso"));
    yarp::os::Time::delay(2);
    EXPECT_TRUE(audioManager->stopMusic());
    EXPECT_FALSE(mockManager->isPlaying("bso"));
    EXPECT_TRUE(audioManager->stop());
}

TEST_F( MockAudioManagerTest, AudioManagerPlaysFx )
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

TEST_F( MockAudioManagerTest, AudioManagerPlaysAllSounds )
{
    ASSERT_NE((AudioManager *)NULL, audioManager);

    ASSERT_TRUE(audioManager->load(sound_bso_realpath, "bso", AudioManager::MUSIC));
    ASSERT_TRUE(audioManager->load(sound_shoot_realpath, "shoot", AudioManager::FX));
    ASSERT_TRUE(audioManager->load(sound_explosion_realpath, "explosion", AudioManager::FX));
    ASSERT_TRUE(audioManager->start());

    EXPECT_TRUE(audioManager->play("bso", true));
    EXPECT_TRUE(audioManager->play("shoot", true));
    EXPECT_TRUE(mockManager->isPlaying("bso"));
    EXPECT_TRUE(mockManager->isPlaying("shoot"));
    yarp::os::Time::delay(1);
    EXPECT_TRUE(audioManager->play("explosion", false));
    EXPECT_TRUE(mockManager->isPlaying("explosion"));
    yarp::os::Time::delay(4);

    EXPECT_TRUE(audioManager->stopMusic());
    EXPECT_FALSE(mockManager->isPlaying("bso"));
    EXPECT_FALSE(mockManager->isPlaying("shoot"));
    EXPECT_FALSE(mockManager->isPlaying("explosion"));
    EXPECT_TRUE(audioManager->stop());
}

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
