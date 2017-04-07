#include "MockInputEventListener.hpp"

rd::MockInputEventListener::MockInputEventListener()
{
    num_keydown_presses = 0;
    num_keyup_presses = 0;
    num_window_events = 0;
}

bool rd::MockInputEventListener::onKeyDown(const Key & k)
{
    num_keydown_presses++;
    stored_keydown_presses.push_back(k);

    return true;
}

bool rd::MockInputEventListener::onKeyUp(const Key & k)
{
    num_keyup_presses++;
    stored_keyup_presses.push_back(k);

    return true;
}

bool rd::MockInputEventListener::onWindowEvent(const WindowEvent & event)
{
    num_window_events++;
    stored_window_events.push_back(event);

    return true;
}

int rd::MockInputEventListener::getNumKeyDownPresses() const
{
    return num_keydown_presses;
}

int rd::MockInputEventListener::getNumKeyUpPresses() const
{
    return num_keyup_presses;
}

int rd::MockInputEventListener::getNumWindowEvents() const
{
    return num_window_events;
}

bool rd::MockInputEventListener::clear()
{
    num_keydown_presses = 0;
    num_keyup_presses = 0;
    num_window_events = 0;

    stored_keydown_presses.clear();
    stored_keyup_presses.clear();
    stored_window_events.clear();

    return true;
}

std::vector<rd::Key> rd::MockInputEventListener::getStoredKeyUpPresses() const
{
    return stored_keyup_presses;
}

std::vector<rd::Key> rd::MockInputEventListener::getStoredKeyDownPresses() const
{
    return stored_keydown_presses;
}

std::vector<rd::WindowEvent> rd::MockInputEventListener::getStoredWindowEvents() const
{
    return stored_window_events;
}
