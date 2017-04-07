// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_FINITE_STATE_MACHINE_HPP__
#define __RD_FINITE_STATE_MACHINE_HPP__

#include <vector>

#include "StateDirector.hpp"

namespace rd{

/**
 * @ingroup StateMachineLib
 *
 * @brief Class implementing a finite state machine
 */
class FiniteStateMachine
{
    public:
        /**
         * @brief Creates a FiniteStateMachine from the states that form it
         * @param stateDirectors vector of StateDirector forming the FiniteStateMachine
         * @param initial_state_id Id of the initial state
         */
        FiniteStateMachine(const std::vector<StateDirector *> & stateDirectors, int initial_state_id);
        virtual ~FiniteStateMachine();

        //! @brief Starts the state machine (running the initial state)
        bool start();
        bool stop();

        //! @brief Returns current active state id
        int getCurrentState() const;

    private:
        FiniteStateMachine();

        std::vector<StateDirector *> stateDirectors;
        int initial_state_id;
};

}

#endif //-- __RD_FINITE_STATE_MACHINE_HPP__

