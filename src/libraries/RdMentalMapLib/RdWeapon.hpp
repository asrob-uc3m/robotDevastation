// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_WEAPON_HPP__
#define __RD_WEAPON_HPP__

#include <string>
#include "RdUtils.hpp"
#include "RdTarget.hpp"
#include "RdPlayer.hpp"
#include "RdAudioManager.hpp"

namespace rd {


/**
 * @ingroup RdMentalMapLib
 *
 * @brief Class that represents a weapon
 *
 */
class RdWeapon
{
public:
    //! @brief Constructs a default, dummy weapon. (That is invalid for the game, by the way)
    RdWeapon();
    RdWeapon(std::string name, int damage, int max_ammo);

    //! @brief Checks if the target is within the weapon's current range and can be hit by it
    bool shoot(RdTarget& target);

    //! @brief Increases the current ammo to max_ammo
    bool reload();


    std::string getName();
    int getDamage();
    int getCurrentAmmo();
    bool setCurrentAmmo(int current_ammo);
    int getMaxAmmo();

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
