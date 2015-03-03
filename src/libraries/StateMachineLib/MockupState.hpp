// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __MOCKUP_STATE_HPP__
#define __MOCKUP_STATE_HPP__

#include "State.hpp"
#include "RdUtils.hpp"

#include <string>
#include <sstream>

#include <yarp/os/Network.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>


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
class MockupState : public State, public yarp::os::BufferedPort<yarp::os::Bottle>
{
    public:
        //! @brief Creates a MockupState assigning it an id
        MockupState(int id);
        virtual ~MockupState();

        //! @brief Sends a message indicating that this function has been called
        virtual bool setup();

        /**
         * @brief Sends a message indicating that this function has been called
         *
         * That message is only sent the first time this funciton has been called
         *
        */
        virtual bool loop();

        //! @brief Sends a message indicating that this function has been called
        virtual bool cleanup();

        //! @brief Returns the internal variable value as condition evaluation result
        virtual int evaluateConditions();

    protected:
        //! @brief Reads an incoming message that changes the internal state of the State
        virtual void onRead(yarp::os::Bottle& b);

    private:
        //! @brief Starts the YARP network and open the required ports
        bool startNetwork(int id);
        //! @brief Stops the YARP network and closes the required ports
        void closeNetwork();

        int id;
        int internal_variable;
        bool first_loop;

        //! @variable Prefix of the ports used to get input and output to the state
        static const std::string debug_port_name;

        yarp::os::BufferedPort<yarp::os::Bottle> debugPort;
        yarp::os::BufferedPort<yarp::os::Bottle> *commandPort;
};


}
#endif // __MOCKUP_STATE_HPP__
