// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __STATE_DIRECTOR_HPP__
#define __STATE_DIRECTOR_HPP__

#include <map>
#include <State.hpp>

namespace rd{

/**
 * @ingroup StateMachineLib
 *
 * @brief A base class for controlling flow of States for a Finite State Machine.
 *
 */
class StateDirector
{
    public:
        StateDirector(State * state);

        virtual bool start() = 0;
        virtual bool stop() = 0;

        virtual bool addTransition( StateDirector * nextState, int condition) = 0;

        virtual bool isActive();

    protected:
        State * state;
        int active;
        std::map<int, StateDirector * > nextStates;
};
}
#endif // __STATE_DIRECTOR_HPP__
