// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_WEAPON_HPP__
#define __RD_WEAPON_HPP__

#include <string>

#include "Target.hpp"

namespace rd {

class Target;

/**
 * @ingroup MentalMapLib
 *
 * @brief Class that represents a weapon
 *
 */
class Weapon
{
public:
    //! @brief Constructs a default, dummy weapon. (That is invalid for the game, by the way)
    Weapon();
    Weapon(std::string name, int damage, int max_ammo);

    //! @brief Checks if the target is within the weapon's current range and can be hit by it
    bool canShootTarget(Target& target);

    //! @brief Increases the current ammo to max_ammo
    bool reload();


    std::string getName() const;
    int getDamage() const;
    int getCurrentAmmo() const;
    bool setCurrentAmmo(int current_ammo);
    int getMaxAmmo() const;

    //! @brief X coordinate of the weapon's scope
    static const int SCOPE_X;
    //! @brief Y coordinate of the weapon's scope
    static const int SCOPE_Y;

private:
    std::string name;

    //! @brief Damage that this weapon can inflict in one shot
    int damage;

    //! @brief Current ammount of ammo
    int current_ammo;

    //! @brief Maximum ammount of ammo
    int max_ammo;
};

}
#endif //__RD_WEAPON_HPP__
