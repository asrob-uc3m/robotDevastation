#include "RdPlayer.hpp"

std::string rd::RdPlayer::getName() const
{
    return name;
}

void rd::RdPlayer::setName(const std::string &value)
{
    name = value;
}

int rd::RdPlayer::getId() const
{
    return id;
}

void rd::RdPlayer::setId(int value)
{
    id = value;
}

int rd::RdPlayer::getHealth() const
{
    return health;
}

void rd::RdPlayer::setHealth(int value)
{
    health = value;
}

int rd::RdPlayer::getMax_health() const
{
    return max_health;
}

void rd::RdPlayer::setMax_health(int value)
{
    max_health = value;
}

int rd::RdPlayer::getTeam_id() const
{
    return team_id;
}

void rd::RdPlayer::setTeam_id(int value)
{
    team_id = value;
}


rd::RdPlayer::RdPlayer(int id, std::string name, int max_health, int team_id)
{
    this->id = id;
    this->name = name;
    this->health = max_health;
    this->max_health = max_health;
    this->team_id = team_id;
}
