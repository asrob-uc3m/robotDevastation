#include "StateDirector.hpp"


rd::StateDirector::StateDirector(rd::State *state)
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

std::string rd::StateDirector::getStateId()
{
    if (state != NULL)
        return state->getStateId();
    else
        return "";
}

bool rd::StateDirector::addTransition(rd::StateDirector *nextState, int condition)
{
    if ( nextStates.find(condition) == nextStates.end())
    {
        RD_DEBUG("Added transition from %s to %s linked to condition %d\n", state->getStateId().c_str(),
                 nextState->getStateId().c_str(), condition);
        nextStates[condition] = nextState;
        return true;
    }
    else
    {
        RD_ERROR("Condition %d already in use in %s\n", condition, state->getStateId().c_str());
        return false;
    }
}

bool rd::StateDirector::isActive()
{
    return active;
}
