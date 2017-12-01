// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "State.hpp"

rd::State::State()
{
    state_id = '\0';
}

rd::State::~State()
{

}

const std::string & rd::State::getStateId() const
{
    return state_id;
}

rd::State *rd::State::getEndState()
{
    return (State *)NULL;
}
