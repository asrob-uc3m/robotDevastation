#include "StateMachineBuilder.hpp"

rd::StateMachineBuilder::StateMachineBuilder()
{
}

bool rd::StateMachineBuilder::setDirectorType(const std::string &type)
{
    return false;
}

int rd::StateMachineBuilder::addState(rd::State *state)
{
    return false;
}

bool rd::StateMachineBuilder::addTransition(int origin_id, int destination_id, int condition)
{
    return false;
}

rd::FiniteStateMachine *rd::StateMachineBuilder::buildStateMachine()
{
    return NULL;
}
