#include "MockupInputEventListener.hpp"

rd::MockupInputEventListener::MockupInputEventListener()
{

}

bool rd::MockupInputEventListener::onKeyDown(rd::RdKey k)
{
    return false;
}

bool rd::MockupInputEventListener::onKeyUp(rd::RdKey k)
{
    return false;
}


int rd::MockupInputEventListener::getNumKeyDownPresses()
{
    return false;
}

int rd::MockupInputEventListener::getNumKeyUpPresses()
{
    return false;
}

bool rd::MockupInputEventListener::clear()
{
    return false;
}

std::vector<rd::RdKey> rd::MockupInputEventListener::getStoredKeyUpPresses()
{

}

std::vector<rd::RdKey> rd::MockupInputEventListener::getStoredKeyDownPresses()
{

}
