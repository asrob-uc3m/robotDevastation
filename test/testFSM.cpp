#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "State.hpp"
#include "MockupState.hpp"
#include "StateDirector.hpp"
#include "YarpStateDirector.hpp"
#include "RdUtils.hpp"

#include <yarp/os/Network.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Port.h>

#include <yarp/os/Time.h>

using namespace rd;

//-- Class for the setup of the enviroment for all the tests
//----------------------------------------------------------------------------------------
//-- This opens the debug port for the FSM to send the messages to check states

class FSMTestEnvironment : public testing::Environment
{
    public:
        FSMTestEnvironment(int argc, char ** argv)
        {
            this->argc = argc;
            this->argv = argv;
        }

        virtual void SetUp()
        {
            //-- Init yarp network & server
            yarp::os::Network::init();
            //yarp::os::Network::runNameServer(argc, argv);
        }

        virtual void TearDown()
        {
            yarp::os::Network::fini();
        }


    private:
        int argc;
        char ** argv;

};



//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class FSMTest : public testing::Test
{
    public:
        virtual void SetUp()
        {

            initPorts();

            state1 = new MockupState(1);
            state2 = new MockupState(2);
            state3 = new MockupState(3);

            stateDirector1 = new YarpStateDirector(state1);
            stateDirector2 = new YarpStateDirector(state2);
            stateDirector3 = new YarpStateDirector(state3);

            //-- Connect states to yarp
            ASSERT_TRUE(yarp::os::Network::connect("/testState/1/status:o", debug_port_name + "/status:i" ));
            ASSERT_TRUE(yarp::os::Network::connect( "/testState/2/status:o", debug_port_name + "/status:i"));
            ASSERT_TRUE(yarp::os::Network::connect("/testState/3/status:o", debug_port_name + "/status:i"));


            ASSERT_TRUE(yarp::os::Network::connect(debug_port_name + "/command:o", "/testState/1/command:i"));
            ASSERT_TRUE(yarp::os::Network::connect(debug_port_name + "/command:o", "/testState/2/command:i"));
            ASSERT_TRUE(yarp::os::Network::connect(debug_port_name + "/command:o", "/testState/3/command:i"));

        }

        virtual void TearDown()
        {
            //-- Disconnect states from yarp
            closePorts();

            delete stateDirector1;
            delete stateDirector2;
            delete stateDirector3;

            stateDirector1 = NULL;
            stateDirector2 = NULL;
            stateDirector3 = NULL;

            delete state1;
            delete state2;
            delete state3;

            state1 = NULL;
            state2 = NULL;
            state3 = NULL;


        }

    void initPorts()
    {
        //-- Setup yarp ports
        ASSERT_TRUE(debugPort.open(debug_port_name + "/status:i"));
        ASSERT_TRUE(commandPort.open(debug_port_name + "/command:o"));
    }

    void closePorts()
    {
        //-- Close yarp ports
        debugPort.interrupt();
        commandPort.interrupt();

        debugPort.close();
        commandPort.close();
    }



    protected:
        State *state1, *state2, *state3;
        StateDirector *stateDirector1, *stateDirector2, *stateDirector3;

        static const std::string debug_port_name;

        yarp::os::Port debugPort;
        yarp::os::Port commandPort;
};

const std::string FSMTest::debug_port_name = "/debug";


//--- Tests ------------------------------------------------------------------------------------------
TEST_F(FSMTest, StateMachineFlowIsCorrect )
{
    RD_INFO("Test Starts!\nAssigning transitions...\n");

    //-- Setup state machine
    stateDirector1->addTransition(stateDirector2, 2);
    stateDirector2->addTransition(stateDirector1, 1);
    stateDirector2->addTransition(stateDirector3, 3);
    stateDirector3->addTransition(stateDirector1, 1);

    //-- Start state machine
    ASSERT_TRUE(stateDirector1->Start());

    RD_WARNING("Activated thread\n");

    //-- Check that init state is active
    ASSERT_TRUE(stateDirector1->isActive());
    ASSERT_FALSE(stateDirector2->isActive());
    ASSERT_FALSE(stateDirector3->isActive());

    //-- Check that the init state passed through setup and init states:
    yarp::os::Bottle debugMsg;
    debugPort.read(debugMsg);
    EXPECT_STREQ("setup", debugMsg.get(0).asString().c_str());
    EXPECT_STREQ("loop", debugMsg.get(1).asString().c_str());

    RD_WARNING("Received data. %d elements.\n", debugMsg.size());

    //-- Send command to pass to state 2
    yarp::os::Bottle state2Cmd;
    state2Cmd.addInt(2);
    commandPort.write(state2Cmd);
    RD_WARNING("Sent command.\n");
    yarp::os::Time::delay(0.2);

    //-- Check that state 2 is active
    ASSERT_FALSE(stateDirector1->isActive());
    ASSERT_TRUE(stateDirector2->isActive());
    ASSERT_FALSE(stateDirector3->isActive());

    //-- Check that the state 1 passed through cleanup
    debugMsg.clear();
    debugPort.read(debugMsg);
    EXPECT_STREQ("cleanup", debugMsg.get(0).asString().c_str());
    EXPECT_STREQ("setup", debugMsg.get(1).asString().c_str());
    EXPECT_STREQ("loop", debugMsg.get(2).asString().c_str());

    //-- Send command to pass to state 3
    yarp::os::Bottle state3Cmd;
    state3Cmd.addInt(3);
    commandPort.write(state3Cmd);
    yarp::os::Time::delay(0.2);

    //-- Check that state 2 is active
    ASSERT_FALSE(stateDirector1->isActive());
    ASSERT_FALSE(stateDirector2->isActive());
    ASSERT_TRUE(stateDirector3->isActive());

    //-- Check that the state 2 passed through cleanup
    debugMsg.clear();
    debugPort.read(debugMsg);
    EXPECT_STREQ("cleanup", debugMsg.get(0).asString().c_str());
    EXPECT_STREQ("setup", debugMsg.get(1).asString().c_str());
    EXPECT_STREQ("loop", debugMsg.get(2).asString().c_str());

    //-- Stop current state
    ASSERT_TRUE(stateDirector3->Stop());
    yarp::os::Time::delay(0.2);

    //-- Check that the state 2 passed through cleanup
    debugMsg.clear();
    debugPort.read(debugMsg);
    EXPECT_STREQ("cleanup", debugMsg.get(0).asString().c_str());
}

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new FSMTestEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}
