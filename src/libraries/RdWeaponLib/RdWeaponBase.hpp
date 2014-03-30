// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_WEAPON_BASE_HPP__
#define __RD_WEAPON_BASE_HPP__

#include <string>
#include "RdMacros.hpp"

namespace rdlib{

class RdWeaponBase
{
    public:
        RdWeaponBase(int weapon_type);

        bool shoot();
        bool reload();
        bool update();

        int getPower();
        int getMaxAmmo();
        int getCurrentAmmo();
        int getShootDelay();
        int getShootCurrentDelay();
        int getReloadDelay();
        int getReloadCurrentDelay();

        static const int RD_WEAPON_LASER_GUN = 0;
        static const int RD_WEAPON_MACHINE_GUN = 1;

    private:
        int power;
        int max_ammo;
        int ammo;
        int shoot_current_delay;
        int shoot_delay;
        int reload_current_delay;
        int reload_delay;

};
} //rdlib

#endif  // __RD_ROBOT_BASE_HPP__
