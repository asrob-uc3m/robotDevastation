#include "Screen.hpp"

rd::Screen::Screen()
{

}

bool rd::Screen::update(const std::string & parameter, const std::string & value)
{
    return true;
}

bool rd::Screen::update(const std::string & parameter, rd::Image value)
{
    return true;
}

bool rd::Screen::update(const std::string & parameter, rd::Player value)
{
    return true;
}

bool rd::Screen::update(const std::string & parameter, const std::vector<rd::Player> & value)
{
    return true;
}

bool rd::Screen::update(const std::string & parameter, const std::vector<rd::Target> & value)
{
    return true;
}

bool rd::Screen::update(const std::string & parameter, rd::Weapon value)
{
    return true;
}
