#include "gtest/gtest.h"
#include <string>

#include "RdAudioManager.hpp"

using namespace rd;

class RdAudioManagerTest : public testing::Test
{
	public:
		void SetUp() 
		{
		}
		
		virtual void TearDown()
		{
		
		}


	protected:
        RdAudioManager audioManager;
        static const std::string sound_bso;
        static const std::string sound_shoot;
        static const std::string sound_explosion;
};


const std::string RdAudioManagerTest::sound_bso = "../../share/sounds/RobotDevastationBSO.mp3";
const std::string RdAudioManagerTest::sound_shoot = "../../share/sounds/01_milshot.wav";
const std::string RdAudioManagerTest::sound_explosion = "../../share/sounds/15_explosion.wav";

TEST_F( RdAudioManagerTest, AudioManagerLoadsAudio)
{
    ASSERT_TRUE(audioManager.load(sound_bso, "bso", 0));
//    ASSERT_TRUE(audioManager.load(sound_shoot, "shoot", RdAudioManager::FX));
//    ASSERT_TRUE(audioManager.load(sound_explosion, "explosion", RdAudioManager::FX));
}

TEST_F( RdAudioManagerTest, AudioManagerPlaysOneSound )
{
    ASSERT_TRUE(audioManager.load(sound_bso, "bso", 0));

    EXPECT_TRUE(audioManager.playMusic("bso", true));
    sleep(2);
    EXPECT_TRUE(audioManager.stopMusic());
}
