// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_STATE_HPP__
#define __RD_STATE_HPP__

#include <string>

namespace rd{

/**
 * @ingroup StateMachineLib
 *
 * @brief Base class for each of the states of a FiniteStateMachine.
 *
 */
class State
{
    public:
        State();
        virtual ~State();

        /**
         * @brief Function executed just before the loop function, when the state is enabled
         * @return False if some problem ocurred
         */
        virtual bool setup() = 0;

        /**
         * @brief Function executed periodically when the state is active
         * @return False if some problem ocurred
         */
        virtual bool loop() = 0;

        /**
         * @brief Function excuted when this state is going to be stopped (due to an error or a transition)
         * @return False if some problem ocurred
         */
        virtual bool cleanup() = 0;

        /**
         * @brief This function is called after each call to loop() in order to know the transition to make
         *
         * An integer value is assigned to each possible transition to identify them.
         *
         * @return Transition selected depending on the conditions of the state
         */
        virtual int evaluateConditions() = 0;


        //! @brief Returns the string that identifies the state
        const std::string & getStateId() const;

        static State *getEndState();

   protected:
        //! @brief Name of the current state
        std::string state_id;

};

}

#endif //-- __RD_STATE_HPP__
