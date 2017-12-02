// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "Screen.hpp"

rd::Screen::Screen()
{
    w = 200; h = 100; //-- Arbitrary size initialization
}

bool rd::Screen::update(const std::string & parameter, const std::string & value)
{
    return true;
}

bool rd::Screen::update(const std::string & parameter, const Image & value)
{
    return true;
}

bool rd::Screen::update(const std::string & parameter, const Player & value)
{
    return true;
}

bool rd::Screen::update(const std::string & parameter, const std::vector<Player> & value)
{
    return true;
}

bool rd::Screen::update(const std::string & parameter, const std::vector<Target> & value)
{
    return true;
}

bool rd::Screen::update(const std::string & parameter, const Weapon & value)
{
    return true;
}

int rd::Screen::getHeight() const
{
    return h;
}

int rd::Screen::getWidth() const
{
    return w;
}
