// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "YarpStateDirector.hpp"

#include <ColorDebug.h>

const int rd::YarpStateDirector::DEFAULT_RATE_MS = 100;

rd::YarpStateDirector::YarpStateDirector(State *state) : StateDirector(state), PeriodicThread(DEFAULT_RATE_MS * 0.001)
{

}

bool rd::YarpStateDirector::Start()
{
    if (state == NULL)
    {
        CD_DEBUG("Null state. Exiting...\n");
        return Stop();
    }

    CD_DEBUG("Starting StateDirector for id %s\n", state->getStateId().c_str());
    active = true;
    if (!state->setup())
    {
        CD_ERROR("Error in state setup for id %s\n", state->getStateId().c_str());
        return false;
    }

    return yarp::os::PeriodicThread::start();
}

bool rd::YarpStateDirector::Stop()
{
    if (state != NULL)
        CD_DEBUG("Stopping StateDirector for id %s\n", state->getStateId().c_str());

    active = false;

    yarp::os::PeriodicThread::askToStop();

    if (state != NULL)
        state->cleanup();

    return true;
}

void rd::YarpStateDirector::run()
{
    //CD_DEBUG("Entering loop in StateDirector with id %s\n", state->getStateId().c_str());
    if ( !state->loop() )
    {
        CD_ERROR("Error in loop. Stopping this state...\n");
        Stop();
    }
    int condition = state->evaluateConditions();

    if (nextStates.find(condition) != nextStates.end())
    {
        Stop();
        nextStates.find(condition)->second->Start();
    }

}
