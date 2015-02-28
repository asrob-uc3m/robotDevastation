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
 * @brief A base class for State Machines.
 *
 */
class FiniteStateMachine
{
    public:
        FiniteStateMachine(std::vector<StateDirector *> stateDirectors, int initial_state_id);
        ~FiniteStateMachine();

        bool setInitialState(int );

        bool start();
        bool stop();

    private:
        std::vector<StateDirector *> stateDirectors;
        int initial_state_id;
};

}

#endif //-- __FINITE_STATE_MACHINE_HPP__

