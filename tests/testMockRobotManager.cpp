// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "gtest/gtest.h"

#include "MockRobotManager.hpp"

namespace rd
{

namespace test
{

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class MockRobotManagerTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            robotManager = new MockRobotManager(robot_name);
        }

        virtual void TearDown()
        {
            delete robotManager;
            robotManager = NULL;
        }

        static const std::string robot_name;

    protected:
        MockRobotManager * robotManager;

};

const std::string MockRobotManagerTest::robot_name = "MockRobot";

//-- Things that are being tested
//-----------------------------------------------------------------------------------------------------
TEST_F(MockRobotManagerTest, MockRobotMoves)
{
    //-- Moving forward (and stop)
    EXPECT_TRUE(robotManager->moveForward(100.0));
    EXPECT_TRUE(robotManager->isMoving());
    EXPECT_EQ(MockRobotManager::FORWARD, robotManager->getMovementDirection());
    EXPECT_TRUE(robotManager->stopMovement());
    EXPECT_FALSE(robotManager->isMoving());
    EXPECT_EQ(MockRobotManager::NONE, robotManager->getMovementDirection());

    //-- Turning left (and stop)
    EXPECT_TRUE(robotManager->turnLeft(100.0));
    EXPECT_TRUE(robotManager->isMoving());
    EXPECT_EQ(MockRobotManager::LEFT, robotManager->getMovementDirection());
    EXPECT_TRUE(robotManager->stopMovement());
    EXPECT_FALSE(robotManager->isMoving());
    EXPECT_EQ(MockRobotManager::NONE, robotManager->getMovementDirection());
}

TEST_F(MockRobotManagerTest, MockRobotCameraMoves)
{
    //-- Tilting down (and stop)
    EXPECT_TRUE(robotManager->tiltDown(100.0));
    EXPECT_TRUE(robotManager->isCameraMoving());
    EXPECT_EQ(MockRobotManager::CAMERA_DOWN, robotManager->getCameraMovementDirection());
    EXPECT_TRUE(robotManager->stopCameraMovement());
    EXPECT_FALSE(robotManager->isCameraMoving());
    EXPECT_EQ(MockRobotManager::CAMERA_NONE, robotManager->getCameraMovementDirection());

    //-- Panning left (and stop)
    EXPECT_TRUE(robotManager->panLeft(100.0));
    EXPECT_TRUE(robotManager->isCameraMoving());
    EXPECT_EQ(MockRobotManager::CAMERA_LEFT, robotManager->getCameraMovementDirection());
    EXPECT_TRUE(robotManager->stopCameraMovement());
    EXPECT_FALSE(robotManager->isCameraMoving());
    EXPECT_EQ(MockRobotManager::CAMERA_NONE, robotManager->getCameraMovementDirection());
}

}  // namespace test

}  // namespace rd

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
