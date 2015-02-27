// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __STATE_MACHINE_BUILDER_HPP__
#define __STATE_MACHINE_BUILDER_HPP__

#include <string>

#include "State.hpp"
#include "StateDirector.hpp"
#include "StateMachine.hpp"

namespace rd {

class StateMachineBuilder
{
    public:
        StateMachineBuilder();

        bool setDirectorType(const std::string& type);

        int addState(State *state);
        bool addTransition(int origin_id, int destination_id, int condition);

        FiniteStateMachine * buildStateMachine();

    private:
        std::vector<State *> states;
        std::vector<StateDirector *> stateDirectors;

};
}
#endif // __STATE_MACHINE_BUILDER_HPP__
