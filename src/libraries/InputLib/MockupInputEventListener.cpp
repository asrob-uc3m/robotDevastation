#include "MockupInputEventListener.hpp"

rd::MockupInputEventListener::MockupInputEventListener()
{
    num_keydown_presses = 0;
    num_keyup_presses = 0;
    num_window_events = 0;
}

bool rd::MockupInputEventListener::onKeyDown(const rd::Key & k)
{
    num_keydown_presses++;
    stored_keydown_presses.push_back(k);

    return true;
}

bool rd::MockupInputEventListener::onKeyUp(const rd::Key & k)
{
    num_keyup_presses++;
    stored_keyup_presses.push_back(k);

    return true;
}

bool rd::MockupInputEventListener::onWindowEvent(const rd::WindowEvent & event)
{
    num_window_events++;
    stored_window_events.push_back(event);

    return true;
}

int rd::MockupInputEventListener::getNumKeyDownPresses()
{
    return num_keydown_presses;
}

int rd::MockupInputEventListener::getNumKeyUpPresses()
{
    return num_keyup_presses;
}

int rd::MockupInputEventListener::getNumWindowEvents()
{
    return num_window_events;
}

bool rd::MockupInputEventListener::clear()
{
    num_keydown_presses = 0;
    num_keyup_presses = 0;
    num_window_events = 0;

    stored_keydown_presses.clear();
    stored_keyup_presses.clear();
    stored_window_events.clear();

    return true;
}

std::vector<rd::Key> rd::MockupInputEventListener::getStoredKeyUpPresses()
{
    return stored_keyup_presses;
}

std::vector<rd::Key> rd::MockupInputEventListener::getStoredKeyDownPresses()
{
    return stored_keydown_presses;
}

std::vector<rd::WindowEvent> rd::MockupInputEventListener::getStoredWindowEvents()
{
    return stored_window_events;
}
