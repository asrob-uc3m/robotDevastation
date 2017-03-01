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
#include <yarp/os/RpcClient.h>
#include <yarp/os/RpcServer.h>

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
class FSMTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            ASSERT_TRUE(rpcClient.open(debug_port_name + "/rpc:c"));

            state1 = new MockupState(1);
            rpcServer1 = dynamic_cast<yarp::os::RpcServer *>(state1);
            yarp::os::PortReader *portReader1 = dynamic_cast<yarp::os::PortReader *>(state1);
            rpcServer1->setReader(*portReader1);
            ASSERT_TRUE(rpcServer1->open("/state1/rpc:s"));

            state2 = new MockupState(2);
            rpcServer2 = dynamic_cast<yarp::os::RpcServer *>(state2);
            yarp::os::PortReader *portReader2 = dynamic_cast<yarp::os::PortReader *>(state2);
            rpcServer2->setReader(*portReader2);
            ASSERT_TRUE(rpcServer2->open("/state2/rpc:s"));

            state3 = new MockupState(3);
            rpcServer3 = dynamic_cast<yarp::os::RpcServer *>(state3);
            yarp::os::PortReader *portReader3 = dynamic_cast<yarp::os::PortReader *>(state3);
            rpcServer3->setReader(*portReader3);
            ASSERT_TRUE(rpcServer3->open("/state3/rpc:s"));

            stateDirector1 = new YarpStateDirector(state1);
            stateDirector2 = new YarpStateDirector(state2);
            stateDirector3 = new YarpStateDirector(state3);

            nullStateDirector = new YarpStateDirector(State::getEndState());
        }

        virtual void TearDown()
        {
            //-- Disconnect states from yarp
            rpcServer1->interrupt();
            rpcServer1->close();

            rpcServer2->interrupt();
            rpcServer2->close();

            rpcServer3->interrupt();
            rpcServer3->close();

            rpcClient.interrupt();
            rpcClient.close();

            delete stateDirector1;
            delete stateDirector2;
            delete stateDirector3;
            delete nullStateDirector;

            stateDirector1 = NULL;
            stateDirector2 = NULL;
            stateDirector3 = NULL;
            nullStateDirector = NULL;
        }

    protected:
        State *state1, *state2, *state3;
        StateDirector *stateDirector1, *stateDirector2, *stateDirector3, *nullStateDirector;

        static const std::string debug_port_name;

        yarp::os::RpcClient rpcClient;
        yarp::os::RpcServer *rpcServer1, *rpcServer2, *rpcServer3;
};

const std::string FSMTest::debug_port_name = "/debug";


//--- Tests ------------------------------------------------------------------------------------------
TEST_F(FSMTest, StateMachineFlowIsCorrect)
{
    RD_INFO("Test Starts!\nAssigning transitions...\n");

    //-- Setup state machine
    stateDirector1->addTransition(stateDirector2, 2);
    stateDirector2->addTransition(stateDirector1, 1);
    stateDirector2->addTransition(stateDirector3, 3);
    stateDirector3->addTransition(stateDirector1, 1);

    //-- Start state machine
    ASSERT_TRUE(stateDirector1->Start());

    //-- Check that state 1 is active
    ASSERT_TRUE(stateDirector1->isActive());
    ASSERT_FALSE(stateDirector2->isActive());
    ASSERT_FALSE(stateDirector3->isActive());

    yarp::os::Bottle command, response;

    //-- Request current point in execution flow of state 1
    ASSERT_TRUE(yarp::os::Network::connect(rpcClient.getName(), rpcServer1->getName()));
    command.addInt(MockupState::REQUEST_STATE);
    rpcClient.write(command, response);

    //-- Check that state 1 passed through setup and loop
    int currentState1 = response.get(0).asInt();
    ASSERT_TRUE(currentState1 & MockupState::STATE_SETUP == MockupState::STATE_SETUP);
    ASSERT_TRUE(currentState1 & MockupState::STATE_LOOP == MockupState::STATE_LOOP);

    //-- Send command to pass to state 2
    command.clear();
    command.addInt(2);
    rpcClient.write(command, response);

    //-- Check that state 1 passed through cleanup
    currentState1 = response.get(0).asInt();
    ASSERT_TRUE(currentState1 & MockupState::STATE_CLEANUP == MockupState::STATE_CLEANUP);

    //-- Wait until state 2 is initialized
    while (!stateDirector2->isActive()) {
        yarp::os::Time::delay(0.5);
    }

    //-- Check that state 2 is active
    ASSERT_FALSE(stateDirector1->isActive());
    ASSERT_TRUE(stateDirector2->isActive());
    ASSERT_FALSE(stateDirector3->isActive());

    //-- Request current point in execution flow of state 1
    ASSERT_TRUE(yarp::os::Network::disconnect(rpcClient.getName(), rpcServer1->getName(), false));
    ASSERT_TRUE(yarp::os::Network::connect(rpcClient.getName(), rpcServer2->getName()));
    command.addInt(MockupState::REQUEST_STATE);
    rpcClient.write(command, response);

    //-- Check that state 2 passed through setup and loop
    int currentState2 = response.get(0).asInt();
    ASSERT_TRUE(currentState2 & MockupState::STATE_SETUP == MockupState::STATE_SETUP);
    ASSERT_TRUE(currentState2 & MockupState::STATE_LOOP == MockupState::STATE_LOOP);

    //-- Send command to pass to state 3
    command.clear();
    command.addInt(3);
    rpcClient.write(command, response);

    //-- Check that state 2 passed through cleanup
    currentState2 = response.get(0).asInt();
    ASSERT_TRUE(currentState2 & MockupState::STATE_CLEANUP == MockupState::STATE_CLEANUP);

    //-- Wait until state 3 is initialized
    while (!stateDirector3->isActive()) {
        yarp::os::Time::delay(0.5);
    }

    //-- Check that state 3 is active
    ASSERT_FALSE(stateDirector1->isActive());
    ASSERT_FALSE(stateDirector2->isActive());
    ASSERT_TRUE(stateDirector3->isActive());

    //-- Request current point in execution flow of state 3
    ASSERT_TRUE(yarp::os::Network::disconnect(rpcClient.getName(), rpcServer2->getName(), false));
    ASSERT_TRUE(yarp::os::Network::connect(rpcClient.getName(), rpcServer3->getName()));
    command.addInt(MockupState::REQUEST_STATE);
    rpcClient.write(command, response);

    //-- Check that state 3 passed through setup and loop
    int currentState3 = response.get(0).asInt();
    ASSERT_TRUE(currentState3 & MockupState::STATE_SETUP == MockupState::STATE_SETUP);
    ASSERT_TRUE(currentState3 & MockupState::STATE_LOOP == MockupState::STATE_LOOP);

    //-- Stop state 3
    ASSERT_TRUE(stateDirector3->Stop());

    //-- Check that state 3 passed through cleanup
    currentState3 = response.get(0).asInt();
    ASSERT_TRUE(currentState3 & MockupState::STATE_CLEANUP == MockupState::STATE_CLEANUP);
}

TEST_F(FSMTest, StateMachineStopsAtNULL)
{
    RD_INFO("Test Starts!\nAssigning transitions...\n");

    //-- Setup state machine
    stateDirector1->addTransition(nullStateDirector, 2);

    //-- Start state machine
    ASSERT_TRUE(stateDirector1->Start());

    //-- Check that state 1 is active
    ASSERT_TRUE(stateDirector1->isActive());

    yarp::os::Bottle command, response;

    //-- Request current point in execution flow of state 1
    ASSERT_TRUE(yarp::os::Network::connect(rpcClient.getName(), rpcServer1->getName()));
    command.addInt(MockupState::REQUEST_STATE);
    rpcClient.write(command, response);

    //-- Check that state 1 passed through setup and loop
    int currentState1 = response.get(0).asInt();
    ASSERT_TRUE(currentState1 & MockupState::STATE_SETUP == MockupState::STATE_SETUP);
    ASSERT_TRUE(currentState1 & MockupState::STATE_LOOP == MockupState::STATE_LOOP);

    //-- Send command to pass to state null (and therefore finish)
    command.clear();
    command.addInt(2);
    rpcClient.write(command, response);

    //-- Check that state 3 passed through cleanup
    ASSERT_TRUE(currentState1 & MockupState::STATE_CLEANUP == MockupState::STATE_CLEANUP);

    //-- Wait until state 1 is finalized
    while (stateDirector1->isActive()) {
        yarp::os::Time::delay(0.5);
    }

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
