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

/**
 * @ingroup StateMachineLib
 *
 * @brief Class that helps with the creation of FiniteStateMachine
 *
 */
class StateMachineBuilder
{
    public:
        StateMachineBuilder();

        /**
         * @brief Select the type of StateDirector to be used
         *
         * The default StateDirector is the YarpStateDirector
         */
        bool setDirectorType(const std::string& type);

        //! @brief Configure the initial state of the FiniteStateMachine
        bool setInitialState(int initial_state_id);


        /**
         * @brief Adds a State to the FiniteStateMachine
         * @param state State to be added
         * @return Id Id assigned to the State added
         */
        int addState(State *state);

        /**
         * @brief Adds a transition from one state to other one depending on a condition
         * @param origin_id Id of the origin state
         * @param destination_id Id of the destination state
         * @param condition Condition to trigger this transition
         */
        bool addTransition(int origin_id, int destination_id, int condition);

        /**
         * @brief Creates a FiniteStateMachine
         *
         * Once the FiniteStateMachine has been configured, this function has to be called
         * to obtain the FiniteStateMachine.start()
         *
         * The returned FiniteStateMachine has to be deleted with the delete operator to
         * released the dynamically allocated memory.
         *
         * @return The created FiniteStateMachine
         */
        FiniteStateMachine * buildStateMachine();

    protected:
        /**
         * @brief Factory method to create a StateDirector from the type string
         *
         * This factory method could be extracted to a factory class to allow new
         * StateDirector without recompiling the StateMachineLib
         */
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
