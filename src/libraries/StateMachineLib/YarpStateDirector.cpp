// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "YarpStateDirector.hpp"

#include <yarp/os/LogStream.h>

const int rd::YarpStateDirector::DEFAULT_RATE_MS = 100;

rd::YarpStateDirector::YarpStateDirector(State *state) : StateDirector(state), PeriodicThread(DEFAULT_RATE_MS * 0.001)
{

}

bool rd::YarpStateDirector::Start()
{
    if (state == NULL)
    {
        yDebug() << "Null state. Exiting...";
        return Stop();
    }

    yDebug() << "Starting StateDirector for id" << state->getStateId();
    active = true;
    if (!state->setup())
    {
        yError() << "Error in state setup for id" << state->getStateId();
        return false;
    }

    return yarp::os::PeriodicThread::start();
}

bool rd::YarpStateDirector::Stop()
{
    if (state != NULL)
        yDebug() << "Stopping StateDirector for id" << state->getStateId();

    active = false;

    yarp::os::PeriodicThread::askToStop();

    if (state != NULL)
        state->cleanup();

    return true;
}

void rd::YarpStateDirector::run()
{
    if ( !state->loop() )
    {
        yError() << "Error in loop. Stopping this state...";
        Stop();
    }
    int condition = state->evaluateConditions();

    if (nextStates.find(condition) != nextStates.end())
    {
        Stop();
        nextStates.find(condition)->second->Start();
    }
}
