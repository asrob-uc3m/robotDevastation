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

bool rd::StateDirector::addTransition(rd::StateDirector *nextState, int condition)
{
    if ( nextStates.find(condition) == nextStates.end())
    {
        RD_DEBUG("Added transition linked to condition %d\n", condition);
        nextStates[condition] = nextState;
        return true;
    }
    else
    {
        RD_ERROR("Condition %d already in use\n", condition);
        return false;
    }
}

bool rd::StateDirector::isActive()
{
    return active;
}
