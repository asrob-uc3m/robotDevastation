#include "gtest/gtest.h"

#include <string>

#include "State.hpp"
#include "MockState.hpp"
#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "Utils.hpp"

#include <yarp/os/Network.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/RpcClient.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/PortReader.h>
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
            ASSERT_TRUE(rpcClient.open(debug_port_name + "/rpc:c"));
        }

        void closePorts()
        {
            //-- Close yarp ports
            rpcClient.interrupt();
            rpcClient.close();
        }

    protected:
        bool awaitStateCheck(const std::string & state_id, const unsigned int reference_state)
        {
            RD_DEBUG("%s\n", state_id.c_str());

            unsigned int retry_count = 0;
            int currentState = 0;
            yarp::os::Bottle command, response;

            do
            {
                if (retry_count >= max_retries)
                    return false;

                yarp::os::Time::delay(delay_s);

                command.clear();
                command.addInt(MockState::REQUEST_STATE);

                rpcClient.write(command, response);
                currentState = response.get(0).asInt();

                retry_count++;
            } while ((currentState & reference_state) != reference_state);

            return true;
        }

        FiniteStateMachine *fsm;

        static const std::string debug_port_name;
        static const unsigned int max_retries;
        static const double delay_s;

        yarp::os::RpcClient rpcClient;
};

const std::string FSMBuilderTest::debug_port_name = "/debug";
const unsigned int FSMBuilderTest::max_retries = 10;
const double FSMBuilderTest::delay_s = 0.5;


//--- Tests ------------------------------------------------------------------------------------------
TEST_F(FSMBuilderTest, StateMachineGeneratedIsCorrect)
{
    //-- Create state machine builder and configure state machine:
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    State *state1 = new MockState(1);
    int state1_id = builder.addState(state1);
    ASSERT_NE(-1, state1_id);

    State *state2 = new MockState(2);
    int state2_id = builder.addState(state2);
    ASSERT_NE(-1, state2_id);

    State *state3 = new MockState(3);
    int state3_id = builder.addState(state3);
    ASSERT_NE(-1, state3_id);

    ASSERT_TRUE(builder.addTransition(state1_id, state2_id, 2));
    ASSERT_TRUE(builder.addTransition(state2_id, state1_id, 1));
    ASSERT_TRUE(builder.addTransition(state2_id, state3_id, 3));
    ASSERT_TRUE(builder.addTransition(state3_id, state1_id, 1));

    ASSERT_TRUE(builder.setInitialState(state1_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    yarp::os::RpcServer *rpcServer1 = dynamic_cast<yarp::os::RpcServer *>(state1);
    yarp::os::PortReader *portReader1 = dynamic_cast<yarp::os::PortReader *>(state1);
    rpcServer1->setReader(*portReader1);
    ASSERT_TRUE(rpcServer1->open("/state1/rpc:s"));

    yarp::os::RpcServer *rpcServer2 = dynamic_cast<yarp::os::RpcServer *>(state2);
    yarp::os::PortReader *portReader2 = dynamic_cast<yarp::os::PortReader *>(state2);
    rpcServer2->setReader(*portReader2);
    ASSERT_TRUE(rpcServer2->open("/state2/rpc:s"));

    yarp::os::RpcServer *rpcServer3 = dynamic_cast<yarp::os::RpcServer *>(state3);
    yarp::os::PortReader *portReader3 = dynamic_cast<yarp::os::PortReader *>(state3);
    rpcServer3->setReader(*portReader3);
    ASSERT_TRUE(rpcServer3->open("/state3/rpc:s"));

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

    //-- Check that state 1 is active
    ASSERT_EQ(state1_id, fsm->getCurrentState());

    //-- Connect RpcClient with RpcServer for state 1
    ASSERT_TRUE(yarp::os::Network::connect(rpcClient.getName(), rpcServer1->getName()));

    //-- Check that state 1 passed through setup and loop
    ASSERT_TRUE(awaitStateCheck(state1->getStateId(), MockState::STATE_SETUP));
    ASSERT_TRUE(awaitStateCheck(state1->getStateId(), MockState::STATE_LOOP));

    //-- Send command to pass to state 2
    yarp::os::Bottle command;
    command.addInt(2);
    rpcClient.write(command);

    //-- Check that state 1 passed through cleanup
    ASSERT_TRUE(awaitStateCheck(state1->getStateId(), MockState::STATE_CLEANUP));

    //-- Check that state 2 is active
    ASSERT_EQ(state2_id, fsm->getCurrentState());

    //-- Disconnect RpcClient from state 1, connect with RpcServer for state 2
    ASSERT_TRUE(yarp::os::Network::disconnect(rpcClient.getName(), rpcServer1->getName(), false));
    ASSERT_TRUE(yarp::os::Network::connect(rpcClient.getName(), rpcServer2->getName()));

    //-- Check that state 2 passed through setup and loop
    ASSERT_TRUE(awaitStateCheck(state2->getStateId(), MockState::STATE_SETUP));
    ASSERT_TRUE(awaitStateCheck(state2->getStateId(), MockState::STATE_LOOP));

    //-- Send command to pass to state 3
    command.clear();
    command.addInt(3);
    rpcClient.write(command);

    //-- Check that state 2 passed through cleanup
    ASSERT_TRUE(awaitStateCheck(state2->getStateId(), MockState::STATE_CLEANUP));

    //-- Check that state 3 is active
    ASSERT_EQ(state3_id, fsm->getCurrentState());

    //-- Disconnect RpcClient from state 2, connect with RpcServer for state 3
    ASSERT_TRUE(yarp::os::Network::disconnect(rpcClient.getName(), rpcServer2->getName(), false));
    ASSERT_TRUE(yarp::os::Network::connect(rpcClient.getName(), rpcServer3->getName()));

    //-- Check that state 3 passed through setup and loop
    ASSERT_TRUE(awaitStateCheck(state3->getStateId(), MockState::STATE_SETUP));
    ASSERT_TRUE(awaitStateCheck(state3->getStateId(), MockState::STATE_LOOP));

    //-- Stop current state
    ASSERT_TRUE(fsm->stop());

    //-- Check that state 3 passed through cleanup
    ASSERT_TRUE(awaitStateCheck(state3->getStateId(), MockState::STATE_CLEANUP));

    //-- Check that no state is active
    ASSERT_EQ(-1, fsm->getCurrentState());
}

TEST_F(FSMBuilderTest, StateMachineGeneratedStopsAtNULL)
{
    //-- Create state machine builder and configure state machine:
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    State *state1 = new MockState(1);
    int state1_id = builder.addState(state1);
    ASSERT_NE(-1, state1_id);

    int state2_id = builder.addState(State::getEndState());
    ASSERT_NE(-1, state2_id);

    ASSERT_TRUE(builder.addTransition(state1_id, state2_id, 2));

    ASSERT_TRUE(builder.setInitialState(state1_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    yarp::os::RpcServer *rpcServer1 = dynamic_cast<yarp::os::RpcServer *>(state1);
    yarp::os::PortReader *portReader1 = dynamic_cast<yarp::os::PortReader *>(state1);
    rpcServer1->setReader(*portReader1);
    ASSERT_TRUE(rpcServer1->open("/state1/rpc:s"));

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

    //-- Check that state 1 is active
    ASSERT_EQ(state1_id, fsm->getCurrentState());

    //-- Connect RpcClient with RpcServer for state 1
    ASSERT_TRUE(yarp::os::Network::connect(rpcClient.getName(), rpcServer1->getName()));

    //-- Check that state 1 passed through setup and loop
    ASSERT_TRUE(awaitStateCheck(state1->getStateId(), MockState::STATE_SETUP));
    ASSERT_TRUE(awaitStateCheck(state1->getStateId(), MockState::STATE_LOOP));

    //-- Send command to pass to state 2
    yarp::os::Bottle command;
    command.addInt(2);
    rpcClient.write(command);

    //-- Check that state 1 passed through cleanup
    ASSERT_TRUE(awaitStateCheck(state1->getStateId(), MockState::STATE_CLEANUP));

    //-- Check that no state is active
    ASSERT_EQ(-1, fsm->getCurrentState());
}


//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new FSMBuilderTestEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}
