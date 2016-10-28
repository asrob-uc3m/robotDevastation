#include "RdScreen.hpp"

rd::RdScreen::RdScreen()
{

}


bool rd::RdScreen::drawScreen(void *screen)
{
    return false;
}

bool rd::RdScreen::update(std::string parameter, std::string value)
{
    return true;
}

bool rd::RdScreen::update(std::string parameter, rd::RdImage value)
{
    return true;
}

bool rd::RdScreen::update(std::string parameter, rd::RdPlayer value)
{
    return true;
}

bool rd::RdScreen::update(std::string parameter, std::vector<rd::RdPlayer> value)
{
    return true;
}

bool rd::RdScreen::update(std::string parameter, std::vector<rd::RdTarget> value)
{
    return true;
}

bool rd::RdScreen::update(std::string parameter, rd::RdWeapon value)
{
    return true;
}
