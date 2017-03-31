#include <sstream>

#include "Player.hpp"

rd::Player::Player()
{
    //-- Default values
    this->id = -1;
    this->name = "Dummy";
    this->health = -1;
    this->max_health = -1;
    this->team_id = -1;
    this->score = -1;
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

std::string rd::Player::getName() const
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

bool rd::Player::getDamageFromWeapon(rd::Weapon &weapon)
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
