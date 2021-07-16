// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "StateDirector.hpp"

#include <yarp/os/LogStream.h>

#include "LogComponent.hpp"

rd::StateDirector::StateDirector(State *state)
{
    this->state = state;
    active = false;
}

rd::StateDirector::~StateDirector()
{
    nextStates.clear();

    delete state;
    state = NULL;
}

std::string rd::StateDirector::getStateId() const
{
    if (state != NULL)
        return state->getStateId();
    else
        return "";
}

bool rd::StateDirector::addTransition(StateDirector *nextState, int condition)
{
    if ( nextStates.find(condition) == nextStates.end())
    {
        yCDebug(RD_SM) << "Added transition from" << state->getStateId() << "to" << nextState->getStateId() << "linked to condition" << condition;
        nextStates[condition] = nextState;
        return true;
    }
    else
    {
        yCError(RD_SM) << "Condition" << condition << "already in use in" << state->getStateId();
        return false;
    }
}

bool rd::StateDirector::isActive() const
{
    return active;
}
