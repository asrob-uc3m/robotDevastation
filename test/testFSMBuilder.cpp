#include "gtest/gtest.h"

#include <string>

#include "State.hpp"
#include "MockupState.hpp"
#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "RdUtils.hpp"

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
        FiniteStateMachine *fsm;

        static const std::string debug_port_name;

        yarp::os::RpcClient rpcClient;
};

const std::string FSMBuilderTest::debug_port_name = "/debug";


//--- Tests ------------------------------------------------------------------------------------------
TEST_F(FSMBuilderTest, StateMachineGeneratedIsCorrect)
{
    //-- Create state machine builder and configure state machine:
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    State *state1 = new MockupState(1);
    int state1_id = builder.addState(state1);
    ASSERT_NE(-1, state1_id);

    State *state2 = new MockupState(2);
    int state2_id = builder.addState(state2);
    ASSERT_NE(-1, state2_id);

    State *state3 = new MockupState(3);
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

    //-- Connect RpcClient with RpcServer for state 1
    ASSERT_TRUE(yarp::os::Network::connect(rpcClient.getName(), rpcServer1->getName()));

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

    yarp::os::Bottle command, response;

    //-- Send command to pass to state 2
    command.addInt(2);
    rpcClient.write(command, response);

    //-- Check that state 1 passed through setup and loop
    int currentState1 = response.get(0).asInt();
    ASSERT_TRUE((currentState1 & MockupState::STATE_SETUP) == MockupState::STATE_SETUP);
    ASSERT_TRUE((currentState1 & MockupState::STATE_LOOP) == MockupState::STATE_LOOP);

    //-- Wait until state 1 passes through cleanup
    do
    {
        yarp::os::Time::delay(0.5);
        command.clear();
        command.addInt(MockupState::REQUEST_STATE);
        rpcClient.write(command, response);
        currentState1 = response.get(0).asInt();
    } while ((currentState1 & MockupState::STATE_CLEANUP) != MockupState::STATE_CLEANUP);

    //-- Check that state2 is active
    ASSERT_EQ(state2_id, fsm->getCurrentState());

    //-- Disconnect RpcClient from state 1, connect with RpcServer for state 2
    ASSERT_TRUE(yarp::os::Network::disconnect(rpcClient.getName(), rpcServer1->getName(), false));
    ASSERT_TRUE(yarp::os::Network::connect(rpcClient.getName(), rpcServer2->getName()));

    //-- Send command to pass to state 3
    command.clear();
    command.addInt(3);
    rpcClient.write(command, response);

    //-- Check that state 2 passed through setup and loop
    int currentState2 = response.get(0).asInt();
    ASSERT_TRUE((currentState2 & MockupState::STATE_SETUP) == MockupState::STATE_SETUP);
    ASSERT_TRUE((currentState2 & MockupState::STATE_LOOP) == MockupState::STATE_LOOP);

    //-- Wait until state 2 passes through cleanup
    do
    {
        yarp::os::Time::delay(0.5);
        command.clear();
        command.addInt(MockupState::REQUEST_STATE);
        rpcClient.write(command, response);
        currentState2 = response.get(0).asInt();
    } while ((currentState2 & MockupState::STATE_CLEANUP) != MockupState::STATE_CLEANUP);

    //-- Check that state3 is active
    ASSERT_EQ(state3_id, fsm->getCurrentState());

    //-- Disconnect RpcClient from state 2, connect with RpcServer for state 3
    ASSERT_TRUE(yarp::os::Network::disconnect(rpcClient.getName(), rpcServer2->getName(), false));
    ASSERT_TRUE(yarp::os::Network::connect(rpcClient.getName(), rpcServer3->getName()));

    //-- Request current point in execution flow of state 3
    command.clear();
    command.addInt(MockupState::REQUEST_STATE);
    rpcClient.write(command, response);

    //-- Check that state 3 passed through setup and loop
    int currentState3 = response.get(0).asInt();
    ASSERT_TRUE((currentState3 & MockupState::STATE_SETUP) == MockupState::STATE_SETUP);
    ASSERT_TRUE((currentState3 & MockupState::STATE_LOOP) == MockupState::STATE_LOOP);

    //-- Stop current state
    ASSERT_TRUE(fsm->stop());

    //-- Request current point in execution flow of state 3
    command.clear();
    command.addInt(MockupState::REQUEST_STATE);
    rpcClient.write(command, response);

    //-- Check that state 3 passed through cleanup
    currentState3 = response.get(0).asInt();
    ASSERT_TRUE((currentState3 & MockupState::STATE_CLEANUP) == MockupState::STATE_CLEANUP);
}

TEST_F(FSMBuilderTest, StateMachineGeneratedStopsAtNULL)
{
    //-- Create state machine builder and configure state machine:
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    State *state1 = new MockupState(1);
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

    //-- Connect RpcClient with RpcServer for state 1
    ASSERT_TRUE(yarp::os::Network::connect(rpcClient.getName(), rpcServer1->getName()));

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

    yarp::os::Bottle command, response;

    //-- Send command to pass to state 2
    command.addInt(2);
    rpcClient.write(command, response);

    //-- Check that state 1 passed through setup and loop
    int currentState1 = response.get(0).asInt();
    ASSERT_TRUE((currentState1 & MockupState::STATE_SETUP) == MockupState::STATE_SETUP);
    ASSERT_TRUE((currentState1 & MockupState::STATE_LOOP) == MockupState::STATE_LOOP);

    //-- Wait until state 1 passes through cleanup
    do
    {
        yarp::os::Time::delay(0.5);
        command.clear();
        command.addInt(MockupState::REQUEST_STATE);
        rpcClient.write(command, response);
        currentState1 = response.get(0).asInt();
    } while ((currentState1 & MockupState::STATE_CLEANUP) != MockupState::STATE_CLEANUP);
}


//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new FSMBuilderTestEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}
