// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __MOCKUP_STATE_HPP__
#define __MOCKUP_STATE_HPP__

#include "State.hpp"
#include "RdUtils.hpp"

#include <string>
#include <sstream>

#include <yarp/os/Bottle.h>
#include <yarp/os/PortReader.h>
#include <yarp/os/RpcServer.h>


namespace rd{

/**
* @ingroup StateMachineLib
*
* @brief Dummy State for testing
*
* Behavior:
*  - Outputs each transition to a state through the debugPort
*  - Saves the value received in the commandPort and returns
* it at condition evaluation
*/
class MockupState : public State, public yarp::os::PortReader, public yarp::os::RpcServer
{
    public:
        //! @brief Creates a MockupState and assigns it an id
        MockupState(int id);
        virtual ~MockupState();

        //! @brief Sets an internal variable indicating that this function has been called
        virtual bool setup();

        //! @brief Sets an internal variable indicating that this function has been called
        virtual bool loop();

        //! @brief Sets an internal variable indicating that this function has been called
        virtual bool cleanup();

        //! @brief Returns the internal variable value as condition evaluation result
        virtual int evaluateConditions();

        //! @brief Reads an incoming message that changes the internal state of the State
        virtual bool read(yarp::os::ConnectionReader & connection);

        //! @brief Indicates that the State has been initialized
        static const unsigned int STATE_INITIAL;

        //! @brief Indicates that the State has called setup()
        static const unsigned int STATE_SETUP;

        //! @brief Indicates that the State has called loop()
        static const unsigned int STATE_LOOP;

        //! @brief Indicates that the State has called cleanup()
        static const unsigned int STATE_CLEANUP;

        //! @brief Request identifier for RPC communication
        static const unsigned int REQUEST_STATE;

    private:
        int id;
        int internal_variable;

        //! @brief Bitmask that stores the execution flow stages this State has passed through
        int state_history;
};


}
#endif // __MOCKUP_STATE_HPP__
