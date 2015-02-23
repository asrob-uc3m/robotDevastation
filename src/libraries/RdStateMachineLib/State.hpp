// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <string>

namespace rd{

/**
 * @ingroup StateMachineLib
 *
 * @brief A base class for States for a Finite State Machine.
 *
 */
class State
{
    public:
        State();
        virtual ~State() {}

        virtual bool setup() = 0;
        virtual bool loop() = 0;
        virtual bool cleanup() = 0;
        virtual int evaluateConditions() = 0;

        std::string getStateId() { return state_id; }

   protected:
        //-- Name of the current state
        std::string state_id;

};

}

#endif //-- __STATE_HPP__
