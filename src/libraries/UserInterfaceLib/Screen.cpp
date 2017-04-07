#include "Screen.hpp"

rd::Screen::Screen()
{
    w = 200; h = 100; //-- Arbitrary size initialization
}

bool rd::Screen::update(const std::string & parameter, const std::string & value)
{
    return true;
}

bool rd::Screen::update(const std::string & parameter, Image value)
{
    return true;
}

bool rd::Screen::update(const std::string & parameter, Player value)
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

bool rd::Screen::update(const std::string & parameter, Weapon value)
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
