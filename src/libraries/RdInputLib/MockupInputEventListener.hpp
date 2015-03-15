// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __MOCKUP_INPUT_EVENT_LISTENER_HPP__
#define __MOCKUP_INPUT_EVENT_LISTENER_HPP__

#include "RdInputEventListener.hpp"

namespace rd{

/**
 * @ingroup RdInputLib
 *
 * @brief Mockup object that receives and stores key presses, maily for testing purposes
 */
class MockupInputEventListener : public RdInputEventListener
{
    public:
        MockupInputEventListener();

        virtual bool onKeyDown( RdKey k );
        virtual bool onKeyUp( RdKey k );

        int getNumKeyDownPresses();
        int getNumKeyUpPresses();
        bool clear();

        std::vector<RdKey> getStoredKeyUpPresses();
        std::vector<RdKey> getStoredKeyDownPresses();

    private:
        int num_keydown_presses;
        std::vector<RdKey> stored_keydown_presses;

        int num_keyup_presses;
        std::vector<RdKey> stored_keyup_presses;

};
}
#endif // __MOCKUP_INPUT_EVENT_LISTENER_HPP__
