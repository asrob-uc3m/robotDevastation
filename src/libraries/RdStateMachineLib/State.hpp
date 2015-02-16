// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __STATE_HPP__
#define __STATE_HPP__

#include "string"

namespace rd{

/**
 * @ingroup RdStateMachineLib
 *
 * @brief A base class for States for a Finite State Machine.
 *
 */
class State
{
    public:
        State();

        virtual bool setup() = 0;
        virtual bool run() = 0;
        virtual bool cleanup() = 0;
        virtual int evaluateConditions() = 0;

        bool start();
        bool stop();

        std::string getStateId();

   protected:
        //-- Name of the current state
        std::string state_id;

};

}

#endif //-- __STATE_HPP__
