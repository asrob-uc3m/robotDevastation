// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "MockState.hpp"

#include <sstream>

#include <yarp/os/Bottle.h>
#include <yarp/os/ConnectionWriter.h>

#include <ColorDebug.h>

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

    CD_DEBUG("Creating MockState with id: %s\n", state_id.c_str());
}

rd::MockState::~MockState()
{
}

bool rd::MockState::setup()
{
    CD_INFO("State with id %d entered in setup() function\n", id);

    internal_variable = 0;
    state_history |= STATE_SETUP;

    return true;
}

bool rd::MockState::loop()
{
    CD_INFO("State with id %d entered in loop() function\n", id);
    if (!(state_history & STATE_LOOP))
    {
        CD_DEBUG("First loop (id %d)\n", id);
        state_history |= STATE_LOOP;
    }

    return true;
}

bool rd::MockState::cleanup()
{
    CD_INFO("State with id %d entered in cleanup() function\n", id);

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
    int received = in.get(0).asInt();

    if (received != REQUEST_STATE)
    {
        CD_INFO("Received: %d at state %s\n", received, state_id.c_str());
        internal_variable = received;
    }

    yarp::os::ConnectionWriter *returnToSender = connection.getWriter();

    if (returnToSender != NULL)
    {
        CD_INFO("Preparing response...\n");
        yarp::os::Bottle out;
        out.addInt(state_history);
        out.write(*returnToSender);
    }

    return true;
}
