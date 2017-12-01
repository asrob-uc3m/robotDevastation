// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "gtest/gtest.h"

#include "MockRobotManager.cpp"

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
TEST_F(MockRobotManagerTest, MockRobotConnectionIsOk)
{
    ASSERT_FALSE(robotManager->isConnected());
    ASSERT_TRUE(robotManager->connect());
    ASSERT_TRUE(robotManager->isConnected());
    ASSERT_TRUE(robotManager->disconnect());
    ASSERT_FALSE(robotManager->isConnected());
}

TEST_F(MockRobotManagerTest, MockRobotCannotMoveIfDisconnected)
{
    EXPECT_FALSE(robotManager->moveForward());
    EXPECT_FALSE(robotManager->moveBackwards());
    EXPECT_FALSE(robotManager->turnLeft());
    EXPECT_FALSE(robotManager->turnRight());
    EXPECT_FALSE(robotManager->stopMovement());

    EXPECT_FALSE(robotManager->tiltUp());
    EXPECT_FALSE(robotManager->tiltDown());
    EXPECT_FALSE(robotManager->panLeft());
    EXPECT_FALSE(robotManager->panRight());
    EXPECT_FALSE(robotManager->isCameraMoving());
}

TEST_F(MockRobotManagerTest, MockRobotCannotMoveIfDisabled)
{
    ASSERT_FALSE(robotManager->isConnected());
    ASSERT_TRUE(robotManager->connect());
    robotManager->setEnabled(false);
    ASSERT_FALSE(robotManager->isEnabled());

    EXPECT_FALSE(robotManager->moveForward());
    EXPECT_FALSE(robotManager->moveBackwards());
    EXPECT_FALSE(robotManager->turnLeft());
    EXPECT_FALSE(robotManager->turnRight());
    EXPECT_FALSE(robotManager->stopMovement());

    EXPECT_FALSE(robotManager->tiltUp());
    EXPECT_FALSE(robotManager->tiltDown());
    EXPECT_FALSE(robotManager->panLeft());
    EXPECT_FALSE(robotManager->panRight());
    EXPECT_FALSE(robotManager->isCameraMoving());

    ASSERT_TRUE(robotManager->disconnect());
    ASSERT_FALSE(robotManager->isConnected());
}

TEST_F(MockRobotManagerTest, MockRobotMoves)
{
    ASSERT_TRUE(robotManager->connect());
    robotManager->setEnabled(true);
    ASSERT_TRUE(robotManager->isEnabled());

    //-- Moving forward (and stop)
    EXPECT_TRUE(robotManager->moveForward());
    EXPECT_TRUE(robotManager->isMoving());
    EXPECT_EQ(MockRobotManager::FORWARD, robotManager->getMovementDirection());
    EXPECT_TRUE(robotManager->stopMovement());
    EXPECT_FALSE(robotManager->isMoving());
    EXPECT_EQ(MockRobotManager::NONE, robotManager->getMovementDirection());

    //-- Moving backwards (and stop)
    EXPECT_TRUE(robotManager->moveBackwards());
    EXPECT_TRUE(robotManager->isMoving());
    EXPECT_EQ(MockRobotManager::BACKWARDS, robotManager->getMovementDirection());
    EXPECT_TRUE(robotManager->stopMovement());
    EXPECT_FALSE(robotManager->isMoving());
    EXPECT_EQ(MockRobotManager::NONE, robotManager->getMovementDirection());

    //-- Turning left (and stop)
    EXPECT_TRUE(robotManager->turnLeft());
    EXPECT_TRUE(robotManager->isMoving());
    EXPECT_EQ(MockRobotManager::LEFT, robotManager->getMovementDirection());
    EXPECT_TRUE(robotManager->stopMovement());
    EXPECT_FALSE(robotManager->isMoving());
    EXPECT_EQ(MockRobotManager::NONE, robotManager->getMovementDirection());

    //-- Turning right (and stop)
    EXPECT_TRUE(robotManager->turnRight());
    EXPECT_TRUE(robotManager->isMoving());
    EXPECT_EQ(MockRobotManager::RIGHT, robotManager->getMovementDirection());
    EXPECT_TRUE(robotManager->stopMovement());
    EXPECT_FALSE(robotManager->isMoving());
    EXPECT_EQ(MockRobotManager::NONE, robotManager->getMovementDirection());

    ASSERT_TRUE(robotManager->disconnect());
}

TEST_F(MockRobotManagerTest, MockRobotCameraMoves)
{
    ASSERT_TRUE(robotManager->connect());
    robotManager->setEnabled(true);
    ASSERT_TRUE(robotManager->isEnabled());

    //-- Tilting up (and stop)
    EXPECT_TRUE(robotManager->tiltUp());
    EXPECT_TRUE(robotManager->isCameraMoving());
    EXPECT_EQ(MockRobotManager::CAMERA_UP, robotManager->getCameraMovementDirection());
    EXPECT_TRUE(robotManager->stopCameraMovement());
    EXPECT_FALSE(robotManager->isCameraMoving());
    EXPECT_EQ(MockRobotManager::CAMERA_NONE, robotManager->getCameraMovementDirection());

    //-- Tilting down (and stop)
    EXPECT_TRUE(robotManager->tiltDown());
    EXPECT_TRUE(robotManager->isCameraMoving());
    EXPECT_EQ(MockRobotManager::CAMERA_DOWN, robotManager->getCameraMovementDirection());
    EXPECT_TRUE(robotManager->stopCameraMovement());
    EXPECT_FALSE(robotManager->isCameraMoving());
    EXPECT_EQ(MockRobotManager::CAMERA_NONE, robotManager->getCameraMovementDirection());

    //-- Panning left (and stop)
    EXPECT_TRUE(robotManager->panLeft());
    EXPECT_TRUE(robotManager->isCameraMoving());
    EXPECT_EQ(MockRobotManager::CAMERA_LEFT, robotManager->getCameraMovementDirection());
    EXPECT_TRUE(robotManager->stopCameraMovement());
    EXPECT_FALSE(robotManager->isCameraMoving());
    EXPECT_EQ(MockRobotManager::CAMERA_NONE, robotManager->getCameraMovementDirection());

    //-- Panning right (and stop)
    EXPECT_TRUE(robotManager->panRight());
    EXPECT_TRUE(robotManager->isCameraMoving());
    EXPECT_EQ(MockRobotManager::CAMERA_RIGHT, robotManager->getCameraMovementDirection());
    EXPECT_TRUE(robotManager->stopCameraMovement());
    EXPECT_FALSE(robotManager->isCameraMoving());
    EXPECT_EQ(MockRobotManager::CAMERA_NONE, robotManager->getCameraMovementDirection());

    ASSERT_TRUE(robotManager->disconnect());
}

}  // namespace test

}  // namespace rd

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
