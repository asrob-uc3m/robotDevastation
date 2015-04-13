#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "RdUtils.hpp"

#include <yarp/os/Network.h>

using namespace rd;

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class InitStateTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            //-- Start YARP network
            yarp::os::Network::init();

        }

        virtual void TearDown()
        {
            //-- Close YARP network
            yarp::os::Network::fini();
        }

    protected:
        FiniteStateMachine *fsm;
};

//--- Tests ------------------------------------------------------------------------------------------
TEST_F(InitStateTest, InitStateWorksCorrectly )
{
    //-- Create mockup objects


    //-- Create fsm with InitState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int init_state_id = builder.addState(new InitState())

}
