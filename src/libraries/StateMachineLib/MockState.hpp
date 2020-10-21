// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_MOCK_STATE_HPP__
#define __RD_MOCK_STATE_HPP__

#include <yarp/os/PortReader.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/ConnectionReader.h>

#include "State.hpp"

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
class MockState : public State, public yarp::os::PortReader, public yarp::os::RpcServer
{
    public:
        //! @brief Creates a MockState and assigns it an id
        MockState(int id);
        virtual ~MockState();

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
        static const int STATE_INITIAL;

        //! @brief Indicates that the State has called setup()
        static const int STATE_SETUP;

        //! @brief Indicates that the State has called loop()
        static const int STATE_LOOP;

        //! @brief Indicates that the State has called cleanup()
        static const int STATE_CLEANUP;

        //! @brief Request identifier for RPC communication
        static const int REQUEST_STATE;

    private:
        int id;
        int internal_variable;

        //! @brief Bitmask that stores the execution flow stages this State has passed through
        int state_history;
};


}
#endif // __RD_MOCK_STATE_HPP__
