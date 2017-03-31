// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_PLAYER_HPP__
#define __RD_PLAYER_HPP__

#include <string>
#include "Weapon.hpp"

namespace rd{

class Weapon;

/**
 * @ingroup MentalMapLib
 *
 * @brief Class that represents a player of this game, with a 'unique' id, name, health, team id and score.
 *
 */
class Player
{
public:
    //! @brief Constructs a default, dummy player. (That is invalid for the game, by the way)
    Player();
    Player ( int id, const std::string & name, int health, int max_health, int team_id, int score);

    int getId() const;
    void setId(int value);

    std::string getName() const;
    void setName(const std::string &value);

    int getHealth() const;
    void setHealth(int value);

    //! @brief Reduces this player's health by an amount specified by the weapon used
    bool getDamageFromWeapon(Weapon& weapon);

    int getMaxHealth() const;
    void setMaxHealth(int value);

    int getTeamId() const;
    void setTeamId(int value);

    int getScore() const;
    void setScore(int value);

    //! @brief Returns a string representing this object
    std::string str() const;

private:
    std::string name;
    int id;
    int health;
    int max_health;
    int team_id;
    int score;

};
}

#endif //__RD_PLAYER_HPP__
