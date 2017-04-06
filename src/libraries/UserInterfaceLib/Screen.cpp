#include "Screen.hpp"

rd::Screen::Screen()
{

}

bool rd::Screen::update(std::string parameter, std::string value)
{
    return true;
}

bool rd::Screen::update(std::string parameter, rd::Image value)
{
    return true;
}

bool rd::Screen::update(std::string parameter, rd::Player value)
{
    return true;
}

bool rd::Screen::update(std::string parameter, std::vector<rd::Player> value)
{
    return true;
}

bool rd::Screen::update(std::string parameter, std::vector<rd::Target> value)
{
    return true;
}

bool rd::Screen::update(std::string parameter, rd::Weapon value)
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
