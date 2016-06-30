#include "RdPlayer.hpp"

rd::RdPlayer::RdPlayer()
{
    //-- Default values
    this->id = -1;
    this->name = "Dummy";
    this->health = -1;
    this->max_health = -1;
    this->team_id = -1;
    this->score = -1;
}

rd::RdPlayer::RdPlayer(int id, std::string name, int health, int max_health, int team_id, int score)
{
    this->id = id;
    this->name = name;
    this->health = health;
    this->max_health = max_health;
    this->team_id = team_id;
    this->score = score;
}

int rd::RdPlayer::getId() const
{
    return id;
}

void rd::RdPlayer::setId(int value)
{
    id = value;
}

std::string rd::RdPlayer::getName() const
{
    return name;
}

void rd::RdPlayer::setName(const std::string &value)
{
    name = value;
}

int rd::RdPlayer::getHealth() const
{
    return health;
}

void rd::RdPlayer::setHealth(int value)
{
    health = value;
}

bool rd::RdPlayer::getDamageFromWeapon(rd::RdWeapon &weapon)
{
    health -= weapon.getDamage();
    if (health < 0)
        health = 0;
    return true;
}

int rd::RdPlayer::getMaxHealth() const
{
    return max_health;
}

void rd::RdPlayer::setMaxHealth(int value)
{
    max_health = value;
}

int rd::RdPlayer::getTeamId() const
{
    return team_id;
}

void rd::RdPlayer::setTeamId(int value)
{
    team_id = value;
}

int rd::RdPlayer::getScore() const
{
    return score;
}

void rd::RdPlayer::setScore(int value)
{
    score = value;
}

std::string rd::RdPlayer::str()
{
    std::stringstream sstream;
    sstream << "ID: " << id << std::endl;
    sstream << "Name: " << name << std::endl;
    sstream << "Health: " << health << "/" << max_health << std::endl;
    sstream << "Team ID: " << team_id << std::endl;
    sstream << "Score: " << score;

    return sstream.str();
}
