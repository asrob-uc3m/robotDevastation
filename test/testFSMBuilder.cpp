#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "State.hpp"
#include "MockupState.hpp"
#include "StateDirector.hpp"
#include "YarpStateDirector.hpp"
#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "RdUtils.hpp"

#include <yarp/os/Network.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Port.h>

#include <yarp/os/Time.h>

using namespace rd;

//-- Class for the setup of the enviroment for all the tests
//----------------------------------------------------------------------------------------
//-- This opens the debug port for the FSM to send the messages to check states

class FSMBuilderTestEnvironment : public testing::Environment
{
    public:
        FSMBuilderTestEnvironment(int argc, char ** argv)
        {
            this->argc = argc;
            this->argv = argv;
        }

        virtual void SetUp()
        {
            //-- Init yarp network & server
            yarp::os::NetworkBase::setLocalMode(true);
            yarp::os::Network::init();
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
class FSMBuilderTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            initPorts();
            fsm = NULL;
        }

        virtual void TearDown()
        {
            closePorts();
            delete fsm;
            fsm = NULL;
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
        FiniteStateMachine *fsm;

        static const std::string debug_port_name;

        yarp::os::BufferedPort<yarp::os::Bottle> debugPort;
        yarp::os::Port commandPort;
};

const std::string FSMBuilderTest::debug_port_name = "/debug";


//--- Tests ------------------------------------------------------------------------------------------
TEST_F(FSMBuilderTest, StateMachineGeneratedIsCorrect )
{
    //-- Create state machine builder and configure state machine:
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int state1_id = builder.addState(new MockupState(1));
    ASSERT_NE(-1, state1_id);

    int state2_id = builder.addState(new MockupState(2));
    ASSERT_NE(-1, state2_id);

    int state3_id = builder.addState(new MockupState(3));
    ASSERT_NE(-1, state3_id);

    ASSERT_TRUE(builder.addTransition(state1_id, state2_id, 2));
    ASSERT_TRUE(builder.addTransition(state2_id, state1_id, 1));
    ASSERT_TRUE(builder.addTransition(state2_id, state3_id, 3));
    ASSERT_TRUE(builder.addTransition(state3_id, state1_id, 1));

    ASSERT_TRUE(builder.setInitialState(state1_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    //-- Connect states to yarp
    ASSERT_TRUE(yarp::os::Network::connect("/testState/1/status:o", debug_port_name + "/status:i" ));
    ASSERT_TRUE(yarp::os::Network::connect( "/testState/2/status:o", debug_port_name + "/status:i"));
    ASSERT_TRUE(yarp::os::Network::connect("/testState/3/status:o", debug_port_name + "/status:i"));

    ASSERT_TRUE(yarp::os::Network::connect(debug_port_name + "/command:o", "/testState/1/command:i"));
    ASSERT_TRUE(yarp::os::Network::connect(debug_port_name + "/command:o", "/testState/2/command:i"));
    ASSERT_TRUE(yarp::os::Network::connect(debug_port_name + "/command:o", "/testState/3/command:i"));

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

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

    //-- Check that state2 is active
    ASSERT_EQ(state2_id, fsm->getCurrentState());

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

    //-- Check that state3 is active
    ASSERT_EQ(state3_id, fsm->getCurrentState());

    //-- Check that the state 3 passed through setup and init states:
    debugMsg = debugPort.read();
    EXPECT_STREQ("setup", debugMsg->get(0).asString().c_str());

    debugMsg = debugPort.read();
    EXPECT_STREQ("loop", debugMsg->get(0).asString().c_str());

    //-- Stop current state
    ASSERT_TRUE(fsm->stop());

    //-- Check that the state 3 passed through cleanup
    debugMsg = debugPort.read();
    EXPECT_STREQ("cleanup", debugMsg->get(0).asString().c_str());
}

TEST_F(FSMBuilderTest, StateMachineGeneratedStopsAtNULL)
{
    //-- Create state machine builder and configure state machine:
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int state1_id = builder.addState(new MockupState(1));
    ASSERT_NE(-1, state1_id);

    int state2_id = builder.addState(State::getEndState());
    ASSERT_NE(-1, state2_id);

    ASSERT_TRUE(builder.addTransition(state1_id, state2_id, 2));

    ASSERT_TRUE(builder.setInitialState(state1_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    //-- Connect states to yarp
    ASSERT_TRUE(yarp::os::Network::connect("/testState/1/status:o", debug_port_name + "/status:i" ));
    ASSERT_TRUE(yarp::os::Network::connect(debug_port_name + "/command:o", "/testState/1/command:i"));

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

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
}


//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new FSMBuilderTestEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}
