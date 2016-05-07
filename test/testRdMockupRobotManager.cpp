#include "gtest/gtest.h"

#include "RdMockupRobotManager.cpp"

using namespace rd;

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class RdMockupRobotManagerTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            robotManager = new RdMockupRobotManager(robot_name);
        }

        virtual void TearDown()
        {
            delete robotManager;
            robotManager = NULL;
        }

        static const std::string robot_name;

    protected:
        RdRobotManager * robotManager;

};

const std::string RdMockupRobotManagerTest::robot_name = "MockupRobot";

//-- Things that are being tested
//-----------------------------------------------------------------------------------------------------
TEST_F(RdMockupRobotManagerTest, RdMockupRobotConnectionIsOk)
{
    ASSERT_FALSE(robotManager->isConnected());
    ASSERT_TRUE(robotManager->connect());
    ASSERT_TRUE(robotManager->isConnected());
    ASSERT_TRUE(robotManager->disconnect());
    ASSERT_FALSE(robotManager->isConnected());
}

TEST_F(RdMockupRobotManagerTest, RdMockupRobotMoves)
{
    ASSERT_TRUE(robotManager->connect());

    //-- Moving forward (and stop)
    EXPECT_TRUE(robotManager->moveForward());
    EXPECT_TRUE(robotManager->isMoving());
    EXPECT_EQ(RdMockupRobotManager::FORWARD, robotManager->getMovementDirection());
    EXPECT_TRUE(robotManager->stopMovement());
    EXPECT_FALSE(robotManager->isMoving());
    EXPECT_EQ(RdMockupRobotManager::NONE, robotManager->getMovementDirection());

    //-- Moving backwards (and stop)
    EXPECT_TRUE(robotManager->moveBackwards());
    EXPECT_TRUE(robotManager->isMoving());
    EXPECT_EQ(RdMockupRobotManager::BACKWARDS, robotManager-getMovementDirection());
    EXPECT_TRUE(robotManager->stopMovement());
    EXPECT_FALSE(robotManager->isMoving());
    EXPECT_EQ(RdMockupRobotManager::NONE, robotManager->getMovementDirection());

    //-- Turning left (and stop)
    EXPECT_TRUE(robotManager->turnLeft());
    EXPECT_TRUE(robotManager->isMoving());
    EXPECT_EQ(RdMockupRobotManager::LEFT, robotManager->getMovementDirection());
    EXPECT_TRUE(robotManager->stopMovement());
    EXPECT_FALSE(robotManager->isMoving());
    EXPECT_EQ(RdMockupRobotManager::NONE, robotManager->getMovementDirection());

    //-- Turning right (and stop)
    EXPECT_TRUE(robotManager->turnRight());
    EXPECT_TRUE(robotManager->isMoving());
    EXPECT_EQ(RdMockupRobotManager::RIGHT, robotManager->getMovementDirection());
    EXPECT_TRUE(robotManager->stopMovement());
    EXPECT_FALSE(robotManager->isMoving());
    EXPECT_EQ(RdMockupRobotManager::NONE, robotManager->getMovementDirection());

    ASSERT_TRUE(robotManager->disconnect());
}

TEST_F(RdMockupRobotManagerTest, RdMockupRobotCameraMoves)
{
    ASSERT_TRUE(robotManager->connect());

    //-- Tilting up (and stop)
    EXPECT_TRUE(robotManager->tiltUp());
    EXPECT_TRUE(robotManager->isCameraMoving());
    EXPECT_EQ(RdMockupRobotManager::CAMERA_UP, robotManager->getCameraMovementDirection());
    EXPECT_TRUE(robotManager->stopCameraMovement());
    EXPECT_FALSE(robotManager->isCameraMoving());
    EXPECT_EQ(RdMockupRobotManager::CAMERA_NONE, robotManager->getCameraMovementDirection());

    //-- Tilting down (and stop)
    EXPECT_TRUE(robotManager->tiltDown());
    EXPECT_TRUE(robotManager->isCameraMoving());
    EXPECT_EQ(RdMockupRobotManager::CAMERA_DOWN, robotManager->getCameraMovementDirection());
    EXPECT_TRUE(robotManager->stopCameraMovement());
    EXPECT_FALSE(robotManager->isCameraMoving());
    EXPECT_EQ(RdMockupRobotManager::CAMERA_NONE, robotManager->getCameraMovementDirection());

    //-- Panning left (and stop)
    EXPECT_TRUE(robotManager->panLeft());
    EXPECT_TRUE(robotManager->isCameraMoving());
    EXPECT_EQ(RdMockupRobotManager::CAMERA_LEFT, robotManager->getCameraMovementDirection());
    EXPECT_TRUE(robotManager->stopCameraMovement());
    EXPECT_FALSE(robotManager->isCameraMoving());
    EXPECT_EQ(RdMockupRobotManager::CAMERA_NONE, robotManager->getCameraMovementDirection());

    //-- Panning right (and stop)
    EXPECT_TRUE(robotManager->panRight());
    EXPECT_TRUE(robotManager->isCameraMoving());
    EXPECT_EQ(RdMockupRobotManager::CAMERA_RIGHT, robotManager->getCameraMovementDirection());
    EXPECT_TRUE(robotManager->stopCameraMovement());
    EXPECT_FALSE(robotManager->isCameraMoving());
    EXPECT_EQ(RdMockupRobotManager::CAMERA_NONE, robotManager->getCameraMovementDirection());

    ASSERT_TRUE(robotManager->disconnect());
}

