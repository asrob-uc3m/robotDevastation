// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __STATE_DIRECTOR_HPP__
#define __STATE_DIRECTOR_HPP__

#include <map>
#include "State.hpp"
#include "Utils.hpp"

namespace rd{

/**
 * @ingroup StateMachineLib
 *
 * @brief A base class for controlling flow of States of a Finite State Machine.
 *
 */
class StateDirector
{
    public:
        //! @brief Creates a StateDirector attached to a State
        StateDirector(State * state);
        virtual ~StateDirector();

        //! @brief Allows access to the id of the attached State
        std::string getStateId();

        /**
         * @brief Function that starts the attached State
         *
         * Calls the setup() function of the State once, and then runs a thread
         * for the loop() function
         */
        virtual bool Start() = 0;

        /**
         * @brief Function to stop the attached State
         *
         * This should stop the thread running the loop() function and call the
         * cleanup() function of the State
         */
        virtual bool Stop() = 0;

        /**
         * @brief Adds a transition to a given state depending on a given condition
         * @param nextState State that will be reached if the condition is fulfilled
         * @param condition Condition that triggers the transition
         */
        virtual bool addTransition( StateDirector * nextState, int condition);

        //! @brief Returns the current state of the state
        virtual bool isActive();

    protected:
        State * state;
        bool active;
        std::map<int, StateDirector * > nextStates;

    private:
        StateDirector() {}
};
}
#endif // __STATE_DIRECTOR_HPP__
