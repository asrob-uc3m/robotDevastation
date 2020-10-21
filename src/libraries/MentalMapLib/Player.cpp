// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "Player.hpp"

#include <sstream>

rd::Player::Player()
{
    //-- Default values
    id = -1;
    name = "Dummy";
    health = -1;
    max_health = -1;
    team_id = -1;
    score = -1;
}

rd::Player::Player(int id, const std::string & name, int health, int max_health, int team_id, int score)
{
    this->id = id;
    this->name = name;
    this->health = health;
    this->max_health = max_health;
    this->team_id = team_id;
    this->score = score;
}

int rd::Player::getId() const
{
    return id;
}

void rd::Player::setId(int value)
{
    id = value;
}

const std::string & rd::Player::getName() const
{
    return name;
}

void rd::Player::setName(const std::string &value)
{
    name = value;
}

int rd::Player::getHealth() const
{
    return health;
}

void rd::Player::setHealth(int value)
{
    health = value;
}

bool rd::Player::getDamageFromWeapon(const Weapon &weapon)
{
    health -= weapon.getDamage();
    if (health < 0)
        health = 0;
    return true;
}

int rd::Player::getMaxHealth() const
{
    return max_health;
}

void rd::Player::setMaxHealth(int value)
{
    max_health = value;
}

int rd::Player::getTeamId() const
{
    return team_id;
}

void rd::Player::setTeamId(int value)
{
    team_id = value;
}

int rd::Player::getScore() const
{
    return score;
}

void rd::Player::setScore(int value)
{
    score = value;
}

std::string rd::Player::str() const
{
    std::stringstream sstream;
    sstream << "ID: " << id << std::endl;
    sstream << "Name: " << name << std::endl;
    sstream << "Health: " << health << "/" << max_health << std::endl;
    sstream << "Team ID: " << team_id << std::endl;
    sstream << "Score: " << score;

    return sstream.str();
}
