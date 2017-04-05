#include "State.hpp"


rd::State::State()
{
    state_id = '\0';
}

rd::State::~State()
{

}

std::string rd::State::getStateId() const
{
    return state_id;
}

rd::State *rd::State::getEndState()
{
    return (State *)NULL;
}
