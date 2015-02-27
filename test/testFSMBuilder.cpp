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
class FSMBuilderTest : public testing::Test
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

    int state2_id = builder.addState(new MockupState(1));
    ASSERT_NE(-1, state2_id);

    int state3_id = builder.addState(new MockupState(3));
    ASSERT_NE(-1, state3_id);

    ASSERT_TRUE(builder.addTransition(state1_id, state2_id, 2));
    ASSERT_TRUE(builder.addTransition(state2_id, state1_id, 1));
    ASSERT_TRUE(builder.addTransition(state2_id, state3_id, 3));
    ASSERT_TRUE(builder.addTransition(state3_id, state1_id, 1));

    FiniteStateMachine *fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

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


    //-- Check that the state 1 passed through setup and init states:
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

    //-- Check that the state 2 passed through setup and init states:
    debugMsg = debugPort.read();
    EXPECT_STREQ("setup", debugMsg->get(0).asString().c_str());

    debugMsg = debugPort.read();
    EXPECT_STREQ("loop", debugMsg->get(0).asString().c_str());

    //-- Stop current state
    ASSERT_TRUE(fsm->stop());

    //-- Check that the state 2 passed through cleanup
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
