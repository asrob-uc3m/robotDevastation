#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "State.hpp"
#include "RdUtils.hpp"

#include <yarp/os/Network.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Port.h>

using namespace rd;


//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class FSMTest : public testing::Test
{
    public:
        virtual void SetUp()
        {

        }

        virtual void TearDown()
        {

        }



    protected:

};

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


//-- Dummy State for testing
//-- Behaviour:
//--     Outputs each transition to a state through the debugPort
//--     Saves the value received in the commandPort and returns
//--     it at condition evaluation

class MockupState : public State, public yarp::os::PortReader
{
    public:
        MockupState(int id);

        virtual bool setup();
        virtual bool loop();
        virtual bool cleanup();
        virtual int evaluateConditions();

    protected:
        virtual bool read(yarp::os::ConnectionReader& connection);

    private:
        static const std::string debug_port_name;

        yarp::os::Port debugPort;
        yarp::os::Port commandPort;
};

const std::string MockupState::debug_port_name = "/testState";

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new FSMTestEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}
