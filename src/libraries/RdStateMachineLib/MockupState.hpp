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

//-- Dummy State for testing
//-- Behaviour:
//--     Outputs each transition to a state through the debugPort
//--     Saves the value received in the commandPort and returns
//--     it at condition evaluation

class MockupState : public State, public yarp::os::BufferedPort<yarp::os::Bottle>
{
    public:
        MockupState(int id);
        virtual ~MockupState();

        virtual bool setup();
        virtual bool loop();
        virtual bool cleanup();
        virtual int evaluateConditions();

    protected:
        virtual void onRead(yarp::os::Bottle& b);

    private:
        bool startNetwork(int id);
        void closeNetwork();

        int id;
        int internal_variable;
        bool first_loop;

        static const std::string debug_port_name;

        yarp::os::Port debugPort;
        yarp::os::BufferedPort<yarp::os::Bottle> *commandPort;
};


}
#endif // __DUMMY_STATE_HPP__
