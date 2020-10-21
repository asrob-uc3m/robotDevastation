// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "StateMachine.hpp"


rd::FiniteStateMachine::FiniteStateMachine(const std::vector<StateDirector *> & stateDirectors, int initial_state_id)
{
    this->stateDirectors = stateDirectors;
    this->initial_state_id = initial_state_id;
}

rd::FiniteStateMachine::~FiniteStateMachine()
{
    //-- Delete state directors:
    for (std::vector<StateDirector *>::iterator it = stateDirectors.begin(); it != stateDirectors.end(); ++it)
    {
        delete *it;
        *it = NULL;
    }

    stateDirectors.clear();
}

bool rd::FiniteStateMachine::start()
{
    return stateDirectors[initial_state_id]->Start();
}

bool rd::FiniteStateMachine::stop()
{
    for (std::vector<StateDirector *>::iterator it = stateDirectors.begin(); it != stateDirectors.end(); ++it)
    {
        if (!(*it)->Stop())
        {
            return false;
        }
    }

    return true;
}

int rd::FiniteStateMachine::getCurrentState() const
{
    for (std::vector<StateDirector *>::const_iterator it = stateDirectors.begin(); it != stateDirectors.end(); ++it)
    {
        if ((*it)->isActive())
        {
            return std::distance(stateDirectors.begin(), it);
        }
    }

    return -1;
}
