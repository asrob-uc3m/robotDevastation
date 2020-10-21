// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_MOCK_INPUT_EVENT_LISTENER_HPP__
#define __RD_MOCK_INPUT_EVENT_LISTENER_HPP__

#include <vector>

#include "InputEventListener.hpp"
#include "Key.hpp"
#include "WindowEvent.hpp"

namespace rd{

/**
 * @ingroup InputLib
 *
 * @brief Mock object that receives and stores key presses, mainly for testing purposes
 */
class MockInputEventListener : public InputEventListener
{
    public:
        MockInputEventListener();

        virtual bool onKeyDown( const Key & k );
        virtual bool onKeyUp( const Key & k );
        virtual bool onWindowEvent( const WindowEvent & event );

        int getNumKeyDownPresses() const;
        int getNumKeyUpPresses() const;
        int getNumWindowEvents() const;

        bool clear();

        const std::vector<Key>& getStoredKeyUpPresses() const;
        const std::vector<Key>& getStoredKeyDownPresses() const;
        const std::vector<WindowEvent>& getStoredWindowEvents() const;

    private:
        int num_keydown_presses;
        std::vector<Key> stored_keydown_presses;

        int num_keyup_presses;
        std::vector<Key> stored_keyup_presses;

        int num_window_events;
        std::vector<WindowEvent> stored_window_events;

};
}
#endif // __RD_MOCK_INPUT_EVENT_LISTENER_HPP__
