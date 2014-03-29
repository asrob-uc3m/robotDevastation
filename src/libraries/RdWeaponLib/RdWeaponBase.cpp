#include "RdWeaponBase.hpp"

rdlib::RdWeaponBase::RdWeaponBase(const int &weapon_type)
{
    if (weapon_type==RD_WEAPON_LASER_GUN)
    {
        max_ammo = 10;
        ammo = max_ammo;
        power = 100;
        shoot_delay = 20;
        reload_delay = 15;
    }
    else if (weapon_type==RD_WEAPON_MACHINE_GUN)
    {
        max_ammo = 100;
        ammo = max_ammo;
        power = 10;
        shoot_delay = 2;
        reload_delay = 25;
    }
    else
    {
        RD_ERROR("Cannot create weapon, specified type does not exist.");
        return;
    }

    shoot_current_delay = 0;
    reload_current_delay = 0;
}


bool rdlib::RdWeaponBase::shoot()
{
    if (ammo <= 0 || shoot_current_delay > 0)
        return false;

    ammo--;
    shoot_current_delay = shoot_delay;
    return true;
}


bool rdlib::RdWeaponBase::reload()
{
    if (reload_current_delay > 0)
        return false;

    ammo = max_ammo;
    reload_current_delay = reload_delay;
    return true;
}


bool rdlib::RdWeaponBase::update()
{
    if ( shoot_current_delay > 0)
        shoot_current_delay--;

    if ( reload_current_delay > 0)
        reload_current_delay--;
}


int rdlib::RdWeaponBase::getPower()
{
    return power;
}


int rdlib::RdWeaponBase::getMaxAmmo()
{
    return max_ammo;
}


int rdlib::RdWeaponBase::getCurrentAmmo()
{
    return ammo;
}

int rdlib::RdWeaponBase::getShootDelay()
{
    return shoot_delay;
}

int rdlib::RdWeaponBase::getShootCurrentDelay()
{
    return shoot_current_delay;
}

int rdlib::RdWeaponBase::getReloadDelay()
{
    return reload_delay;
}

int rdlib::RdWeaponBase::getReloadCurrentDelay()
{
    return reload_current_delay;
}
