// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "MockState.hpp"

#include <sstream>

#include <yarp/os/Bottle.h>
#include <yarp/os/ConnectionWriter.h>
#include <yarp/os/LogStream.h>

const int rd::MockState::STATE_INITIAL = 0;
const int rd::MockState::STATE_SETUP = 1;
const int rd::MockState::STATE_LOOP = 2;
const int rd::MockState::STATE_CLEANUP = 4;

const int rd::MockState::REQUEST_STATE = 0;

rd::MockState::MockState(int id)
{
    //-- Set class parameters:
    this->id = id;

    std::stringstream ss;
    ss << "MockState #" << id;
    state_id = ss.str();

    internal_variable = -1;
    state_history = STATE_INITIAL;

    yDebug() << "Creating MockState with id:" << state_id;
}

rd::MockState::~MockState()
{
}

bool rd::MockState::setup()
{
    yInfo() << "State with id" << id << "entered in setup() function";

    internal_variable = 0;
    state_history |= STATE_SETUP;

    return true;
}

bool rd::MockState::loop()
{
    yInfo() << "State with id" << id << "entered in loop() function";
    if (!(state_history & STATE_LOOP))
    {
        yDebug() << "First loop, id:" << id;
        state_history |= STATE_LOOP;
    }

    return true;
}

bool rd::MockState::cleanup()
{
    yInfo() << "State with id" << id << "entered in cleanup() function";

    internal_variable = -1;
    state_history |= STATE_CLEANUP;

    return true;
}

int rd::MockState::evaluateConditions()
{
    return internal_variable;
}

bool rd::MockState::read(yarp::os::ConnectionReader & connection)
{
    yarp::os::Bottle in;
    in.read(connection);
    int received = in.get(0).asInt32();

    if (received != REQUEST_STATE)
    {
        yInfo() << "Received" << received << "at state" << state_id;
        internal_variable = received;
    }

    yarp::os::ConnectionWriter *returnToSender = connection.getWriter();

    if (returnToSender != NULL)
    {
        yInfo() << "Preparing response...";
        yarp::os::Bottle out;
        out.addInt32(state_history);
        out.write(*returnToSender);
    }

    return true;
}
