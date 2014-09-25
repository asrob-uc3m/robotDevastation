#include "RdWeapon.hpp"


const int rd::RdWeapon::SCOPE_X = 640/2;
const int rd::RdWeapon::SCOPE_Y = 480/2;

rd::RdWeapon::RdWeapon()
{
    //-- Dummy weapon
    name = "dummy";
    damage = -1;
    max_ammo = -1;
    current_ammo = -1;
}

rd::RdWeapon::RdWeapon(std::string name, int damage, int max_ammo)
{
    this->name = name;
    this->damage = damage;
    this->max_ammo = max_ammo;
    this->current_ammo = max_ammo;
}

bool rd::RdWeapon::shoot(rd::RdTarget &target)
{
    //-- Calculate target limits:
    int target_top_left_x = target.getPos().x - target.getDimensions().x / 2;
    int target_top_left_y = target.getPos().y - target.getDimensions().y / 2;

    int target_bottom_right_x = target.getPos().x + target.getDimensions().x / 2;
    int target_bottom_right_y = target.getPos().y + target.getDimensions().y / 2;

    //-- Check if the scope is within target limits:
    if ( target_top_left_x <= SCOPE_X && target_top_left_y <= SCOPE_Y
         && target_bottom_right_x >= SCOPE_X && target_bottom_right_y >= SCOPE_Y)
    {
        return true;
    }
    else
        return false;
}

bool rd::RdWeapon::reload()
{
    current_ammo = max_ammo;
    return true;
}

std::string rd::RdWeapon::getName()
{
    return name;
}

int rd::RdWeapon::getDamage()
{
    return damage;
}

int rd::RdWeapon::getCurrentAmmo()
{
    return current_ammo;
}

bool rd::RdWeapon::setCurrentAmmo(int current_ammo)
{
    this->current_ammo = current_ammo;
    return true;
}

int rd::RdWeapon::getMaxAmmo()
{
    return max_ammo;
}
