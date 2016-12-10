// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __MOCKUP_INPUT_EVENT_LISTENER_HPP__
#define __MOCKUP_INPUT_EVENT_LISTENER_HPP__

#include "RdInputEventListener.hpp"

namespace rd{

/**
 * @ingroup RdInputLib
 *
 * @brief Mockup object that receives and stores key presses, mainly for testing purposes
 */
class MockupInputEventListener : public RdInputEventListener
{
    public:
        MockupInputEventListener();

        virtual bool onKeyDown( const RdKey & k );
        virtual bool onKeyUp( const RdKey & k );
        virtual bool onWindowEvent( const RdWindowEvent & event );

        int getNumKeyDownPresses();
        int getNumKeyUpPresses();
        int getNumWindowEvents();

        bool clear();

        std::vector<RdKey> getStoredKeyUpPresses();
        std::vector<RdKey> getStoredKeyDownPresses();
        std::vector<RdWindowEvent> getStoredWindowEvents();

    private:
        int num_keydown_presses;
        std::vector<RdKey> stored_keydown_presses;

        int num_keyup_presses;
        std::vector<RdKey> stored_keyup_presses;

        int num_window_events;
        std::vector<RdWindowEvent> stored_window_events;

};
}
#endif // __MOCKUP_INPUT_EVENT_LISTENER_HPP__
