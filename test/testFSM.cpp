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
            nullStateDirector = new YarpStateDirector(State::getEndState());

            //-- Connect states to yarp
            ASSERT_TRUE(yarp::os::Network::connect("/testState/1/status:o", debug_port_name + "/status:i" ));
            ASSERT_TRUE(yarp::os::Network::connect( "/testState/2/status:o", debug_port_name + "/status:i"));
            ASSERT_TRUE(yarp::os::Network::connect("/testState/3/status:o", debug_port_name + "/status:i"));

            ASSERT_TRUE(yarp::os::Network::connect(debug_port_name + "/command:o", "/testState/1/command:i"));
            ASSERT_TRUE(yarp::os::Network::connect(debug_port_name + "/command:o", "/testState/2/command:i"));
            ASSERT_TRUE(yarp::os::Network::connect(debug_port_name + "/command:o", "/testState/3/command:i"));

//            ASSERT_TRUE(yarp::os::Network::connect("/testState/1/status:o", "/input"));
//            ASSERT_TRUE(yarp::os::Network::connect("/output", "/testState/1/command:i"));
//            ASSERT_TRUE(yarp::os::Network::connect("/testState/2/status:o", "/input"));
//            ASSERT_TRUE(yarp::os::Network::connect("/output", "/testState/2/command:i"));
//            ASSERT_TRUE(yarp::os::Network::connect("/testState/3/status:o", "/input"));
//            ASSERT_TRUE(yarp::os::Network::connect("/output", "/testState/3/command:i"));
        }

        virtual void TearDown()
        {
            //-- Disconnect states from yarp
            closePorts();

            delete stateDirector1;
            delete stateDirector2;
            delete stateDirector3;
            delete nullStateDirector;

            stateDirector1 = NULL;
            stateDirector2 = NULL;
            stateDirector3 = NULL;
            nullStateDirector = NULL;
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
        StateDirector *stateDirector1, *stateDirector2, *stateDirector3,
        *nullStateDirector;

        static const std::string debug_port_name;

        yarp::os::BufferedPort<yarp::os::Bottle> debugPort;
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

    //-- Check that init state is active
    ASSERT_TRUE(stateDirector1->isActive());
    ASSERT_FALSE(stateDirector2->isActive());
    ASSERT_FALSE(stateDirector3->isActive());

    //-- Check that the init state passed through setup and init states:
    yarp::os::Bottle *debugMsg = debugPort.read();
    EXPECT_STREQ("setup", debugMsg->get(0).asString().c_str());

    debugMsg = debugPort.read();
    EXPECT_STREQ("loop", debugMsg->get(0).asString().c_str());

    //-- Send command to pass to state 2
    yarp::os::Bottle state2Cmd;
    state2Cmd.addInt(2);
    commandPort.write(state2Cmd);

    debugMsg = debugPort.read();
    EXPECT_STREQ("cleanup", debugMsg->get(0).asString().c_str());

    //-- Check that state 2 is active
    ASSERT_FALSE(stateDirector1->isActive());
    ASSERT_TRUE(stateDirector2->isActive());
    ASSERT_FALSE(stateDirector3->isActive());

    //-- Check that the state 2 passed through setup and init states:
    debugMsg = debugPort.read();
    EXPECT_STREQ("setup", debugMsg->get(0).asString().c_str());

    debugMsg = debugPort.read();
    EXPECT_STREQ("loop", debugMsg->get(0).asString().c_str());

    //-- Send command to pass to state 3
    yarp::os::Bottle state3Cmd;
    state3Cmd.addInt(3);
    commandPort.write(state3Cmd);
    debugMsg = debugPort.read();
    EXPECT_STREQ("cleanup", debugMsg->get(0).asString().c_str());

    //-- Check that state 3 is active
    ASSERT_FALSE(stateDirector1->isActive());
    ASSERT_FALSE(stateDirector2->isActive());
    ASSERT_TRUE(stateDirector3->isActive());

    //-- Check that the state 3 passed through setup and init states:
    debugMsg = debugPort.read();
    EXPECT_STREQ("setup", debugMsg->get(0).asString().c_str());

    debugMsg = debugPort.read();
    EXPECT_STREQ("loop", debugMsg->get(0).asString().c_str());

    //-- Stop current state
    ASSERT_TRUE(stateDirector3->Stop());

    //-- Check that the state 3 passed through cleanup
    debugMsg = debugPort.read();
    EXPECT_STREQ("cleanup", debugMsg->get(0).asString().c_str());
}

TEST_F(FSMTest, StateMachineStopsAtNULL )
{
    RD_INFO("Test Starts!\nAssigning transitions...\n");

    //-- Setup state machine

    stateDirector1->addTransition(nullStateDirector, 2);

    //-- Start state machine
    ASSERT_TRUE(stateDirector1->Start());

    //-- Check that init state is active
    ASSERT_TRUE(stateDirector1->isActive());

    //-- Check that the init state passed through setup and init states:
    yarp::os::Bottle *debugMsg = debugPort.read();
    EXPECT_STREQ("setup", debugMsg->get(0).asString().c_str());

    debugMsg = debugPort.read();
    EXPECT_STREQ("loop", debugMsg->get(0).asString().c_str());

    //-- Send command to pass to state  null (and therefore finish)
    yarp::os::Bottle state2Cmd;
    state2Cmd.addInt(2);
    commandPort.write(state2Cmd);

    debugMsg = debugPort.read();
    EXPECT_STREQ("cleanup", debugMsg->get(0).asString().c_str());

    //-- Check that state 1 is not active
    ASSERT_FALSE(stateDirector1->isActive());
}

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new FSMTestEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}
