// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "Weapon.hpp"

const int rd::Weapon::SCOPE_X = 640/2;
const int rd::Weapon::SCOPE_Y = 480/2;

rd::Weapon::Weapon()
{
    //-- Dummy weapon
    name = "dummy";
    damage = -1;
    max_ammo = -1;
    current_ammo = -1;
}

rd::Weapon::Weapon(const std::string & name, int damage, int max_ammo)
{
    this->name = name;
    this->damage = damage;
    this->max_ammo = max_ammo;
    current_ammo = max_ammo;
}

bool rd::Weapon::canShootTarget(const Target &target)
{
    //-- Calculate target limits:
    int target_top_left_x = target.getPos().getX() - target.getDimensions().getX() / 2;
    int target_top_left_y = target.getPos().getY() - target.getDimensions().getY() / 2;

    int target_bottom_right_x = target.getPos().getX() + target.getDimensions().getX() / 2;
    int target_bottom_right_y = target.getPos().getY() + target.getDimensions().getY() / 2;

    //-- Check if the scope is within target limits:
    if ( target_top_left_x <= SCOPE_X && target_top_left_y <= SCOPE_Y
         && target_bottom_right_x >= SCOPE_X && target_bottom_right_y >= SCOPE_Y)
    {
        return true;
    }
    else
        return false;
}

bool rd::Weapon::reload()
{
    current_ammo = max_ammo;
    return true;
}

const std::string & rd::Weapon::getName() const
{
    return name;
}

int rd::Weapon::getDamage() const
{
    return damage;
}

int rd::Weapon::getCurrentAmmo() const
{
    return current_ammo;
}

bool rd::Weapon::setCurrentAmmo(int current_ammo)
{
    this->current_ammo = current_ammo;
    return true;
}

int rd::Weapon::getMaxAmmo() const
{
    return max_ammo;
}
