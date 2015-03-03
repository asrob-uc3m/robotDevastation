// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __FINITE_STATE_MACHINE_HPP__
#define __FINITE_STATE_MACHINE_HPP__

#include "StateDirector.hpp"

namespace rd{

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdStateMachineLib
 *
 * @brief The RdStateMachineLib library contains the \ref RdStateMachine base class and derived classes.
 */

/**
 * @ingroup RdStateMachineLib
 *
 * @brief Class implementing a finite state machine
 *
 */
class FiniteStateMachine
{
    public:
        /**
         * @brief Creates a FiniteStateMachine from the states that form it
         * @param stateDirectors vector of StateDirector forming the FiniteStateMachine
         * @param initial_state_id Id of the initial state
         */
        FiniteStateMachine(std::vector<StateDirector *> stateDirectors, int initial_state_id);
        ~FiniteStateMachine();

        //! @brief Starts the state machine (running the initial state)
        bool start();
        bool stop();

    private:
        FiniteStateMachine();

        std::vector<StateDirector *> stateDirectors;
        int initial_state_id;
};

}

#endif //-- __FINITE_STATE_MACHINE_HPP__

