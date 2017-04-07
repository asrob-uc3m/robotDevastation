// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_STATE_DIRECTOR_HPP__
#define __RD_STATE_DIRECTOR_HPP__

#include <map>

#include "State.hpp"

namespace rd{

/**
 * @ingroup StateMachineLib
 *
 * @brief A base class for controlling flow of States of a Finite State Machine.
 */
class StateDirector
{
    public:
        //! @brief Creates a StateDirector attached to a State
        StateDirector(State * state);
        virtual ~StateDirector();

        //! @brief Allows access to the id of the attached State
        std::string getStateId() const;

        /**
         * @brief Function that starts the attached State
         *
         * Calls the State.setup() function once, and then runs a thread
         * for the State.loop() function.
         */
        virtual bool Start() = 0;

        /**
         * @brief Function to stop the attached State
         *
         * This should stop thread running the State.loop() function and call the
         * State.cleanup() function.
         */
        virtual bool Stop() = 0;

        /**
         * @brief Adds a transition to a given state depending on a given condition
         * @param nextState State that will be reached if the condition is fulfilled
         * @param condition Condition that triggers the transition
         */
        virtual bool addTransition( StateDirector * nextState, int condition);

        //! @brief Returns the current state of the State
        virtual bool isActive() const;

    protected:
        State * state;
        bool active;
        std::map<int, StateDirector * > nextStates;
};
}
#endif // __RD_STATE_DIRECTOR_HPP__
