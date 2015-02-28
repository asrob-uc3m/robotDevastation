// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __STATE_MACHINE_BUILDER_HPP__
#define __STATE_MACHINE_BUILDER_HPP__

#include <string>
#include <map>
#include <utility>

#include "State.hpp"
#include "StateDirector.hpp"
#include "YarpStateDirector.hpp"
#include "StateMachine.hpp"

namespace rd {

class StateMachineBuilder
{
    public:
        StateMachineBuilder();

        bool setDirectorType(const std::string& type);
        bool setInitialState(int initial_state_id);

        int addState(State *state);
        bool addTransition(int origin_id, int destination_id, int condition);

        FiniteStateMachine * buildStateMachine();

    protected:
        StateDirector * createStateDirector(const std::string &type, State* state);

    private:
        std::vector<State *> states;
        std::vector<StateDirector *> stateDirectors;
        /**
         * @brief Stores the transitions of the fsm
         *
         * The format is the following:
         *  { origin_state:[(destination_state, condition)]}
         */
        std::map< int, std::vector<std::pair<int, int> > > transition_table;

        std::string type;
        int initial_state_id;
};
}
#endif // __STATE_MACHINE_BUILDER_HPP__
