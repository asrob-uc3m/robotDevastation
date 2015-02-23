#include "StateDirector.hpp"


rd::StateDirector::StateDirector(rd::State *state)
{
    this->state = state;
    active = false;
}

bool rd::StateDirector::isActive()
{
    return active;
}
