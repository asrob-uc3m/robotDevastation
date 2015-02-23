#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "State.hpp"
#include "StateDirector.hpp"
#include "YarpStateDirector.hpp"
#include "RdUtils.hpp"

#include <yarp/os/Network.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Port.h>
#include <yarp/os/Time.h>

using namespace rd;

//-- Dummy State for testing
//-- Behaviour:
//--     Outputs each transition to a state through the debugPort
//--     Saves the value received in the commandPort and returns
//--     it at condition evaluation

class MockupState : public State, public yarp::os::PortReader
{
    public:
        MockupState(int id): id(id) { internal_variable = -1; first_loop = true;}

        virtual bool setup()
        {
            yarp::os::Bottle bottle;
            bottle.addString("setup");
            debugPort.write(bottle);

            internal_variable = 0;
        }

        virtual bool loop()
        {
            if(first_loop)
            {
                yarp::os::Bottle bottle;
                bottle.addString("loop");
                debugPort.write(bottle);
                first_loop = false;
            }
        }

        virtual bool cleanup()
        {
            yarp::os::Bottle bottle;
            bottle.addString("cleanup");
            debugPort.write(bottle);

            internal_variable = -1;
            first_loop = true;
        }

        virtual int evaluateConditions()
        {
            return internal_variable;
        }

    protected:
        virtual bool read(yarp::os::ConnectionReader& connection)
        {
            yarp::os::Bottle in;
            in.read(connection);

            internal_variable = in.get(0).asInt();

            std::cout << "Received: " << internal_variable << std::endl;
        }

    private:
        int id;
        int internal_variable;
        bool first_loop;

        static const std::string debug_port_name;

        yarp::os::Port debugPort;
        yarp::os::Port commandPort;
};

const std::string MockupState::debug_port_name = "/testState";


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
            yarp::os::Network::runNameServer(argc, argv);

            //-- Setup yarp ports
            debugPort.open(debug_port_name + "/status:i");
            commandPort.open(debug_port_name + "/command:o");
        }

        virtual void TearDown()
        {
            //-- Close yarp ports
            debugPort.interrupt();
            commandPort.interrupt();

            debugPort.close();
            commandPort.close();

            yarp::os::Network::fini();
        }

        static const std::string debug_port_name;

        yarp::os::Port debugPort;
        yarp::os::Port commandPort;

    private:
        int argc;
        char ** argv;

};

const std::string FSMTestEnvironment::debug_port_name = "/debug";

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class FSMTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            state1 = new MockupState(1);
            state2 = new MockupState(2);
            state3 = new MockupState(3);

            stateDirector1 = new YarpStateDirector(state1);
            stateDirector2 = new YarpStateDirector(state2);
            stateDirector3 = new YarpStateDirector(state3);

            //-- Connect states to yarp
        }

        virtual void TearDown()
        {
            //-- Disconnect states from yarp

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



    protected:
        State *state1, *state2, *state3;
        StateDirector *stateDirector1, *stateDirector2, *stateDirector3;

};




//--- Tests ------------------------------------------------------------------------------------------
TEST_F(FSMTest, StateMachineFlowIsCorrect )
{
    //-- Setup state machine
    stateDirector1->addTransition(stateDirector2, 2);
    stateDirector2->addTransition(stateDirector1, 1);
    stateDirector2->addTransition(stateDirector3, 3);
    stateDirector3->addTransition(stateDirector1, 1);

    //-- Start state machine
    ASSERT_TRUE(stateDirector1->start());

    //-- Check that init state is active
    ASSERT_TRUE(stateDirector1->isActive());
    ASSERT_FALSE(stateDirector2->isActive());
    ASSERT_FALSE(stateDirector3->isActive());

    //-- Check that the init state passed through setup and init states:
    yarp::os::Bottle debugMsg;
    debugPort.read(debugMsg);
    EXPECT_STREQ("setup", debugMsg.get(0).asString());
    EXPECT_STREQ("loop", debugMsg.get(1).asString());

    //-- Send command to pass to state 2
    yarp::os::Bottle state2Cmd;
    state2Cmd.addInt(2);
    commandPort.write(state2Cmd);
    yarp::os::Time::delay(0.2);

    //-- Check that state 2 is active
    ASSERT_FALSE(stateDirector1->isActive());
    ASSERT_TRUE(stateDirector2->isActive());
    ASSERT_FALSE(stateDirector3->isActive());

    //-- Check that the state 1 passed through cleanup
    debugMsg.clear();
    debugPort.read(debugMsg);
    EXPECT_STREQ("cleanup", debugMsg.get(0).asString());
    EXPECT_STREQ("setup", debugMsg.get(1).asString());
    EXPECT_STREQ("loop", debugMsg.get(2).asString());

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
    EXPECT_STREQ("cleanup", debugMsg.get(0).asString());
    EXPECT_STREQ("setup", debugMsg.get(1).asString());
    EXPECT_STREQ("loop", debugMsg.get(2).asString());

    //-- Stop current state
    ASSERT_TRUE(stateDirector3->stop());
    yarp::os::Time::delay(0.2);

    //-- Check that the state 2 passed through cleanup
    debugMsg.clear();
    debugPort.read(debugMsg);
    EXPECT_STREQ("cleanup", debugMsg.get(0).asString());
}

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new FSMTestEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}
