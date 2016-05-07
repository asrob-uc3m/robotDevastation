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
}

TEST_F(RdMockupRobotManagerTest, RdMockupRobotMoves)
{
}

TEST_F(RdMockupRobotManagerTest, RdMockupRobotCameraMoves)
{
}

