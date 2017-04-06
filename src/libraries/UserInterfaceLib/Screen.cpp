#include "Screen.hpp"

rd::Screen::Screen()
{
    w = 200; h = 100; //-- Arbitrary size initialization
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
