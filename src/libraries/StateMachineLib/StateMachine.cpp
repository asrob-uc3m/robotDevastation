// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "StateMachine.hpp"


rd::FiniteStateMachine::FiniteStateMachine(const std::vector<rd::StateDirector *> & stateDirectors, int initial_state_id)
{
    this->stateDirectors = stateDirectors;
    this->initial_state_id = initial_state_id;
}

rd::FiniteStateMachine::~FiniteStateMachine()
{
    //-- Delete state directors:
    for (int i = 0; i < stateDirectors.size(); i++)
    {
        delete stateDirectors[i];
        stateDirectors[i] = NULL;
    }

    stateDirectors.clear();
}

bool rd::FiniteStateMachine::start()
{
    return stateDirectors[initial_state_id]->Start();
}

bool rd::FiniteStateMachine::stop()
{
    for (int i = 0; i < stateDirectors.size(); i++)
    {
        if ( !stateDirectors[i]->Stop())
            return false;
    }

    return true;
}

int rd::FiniteStateMachine::getCurrentState() const
{
    int i = 0;
    while (i < stateDirectors.size())
    {
        if ( stateDirectors[i]->isActive())
            return i;
        i++;
    }
    return -1;
}

