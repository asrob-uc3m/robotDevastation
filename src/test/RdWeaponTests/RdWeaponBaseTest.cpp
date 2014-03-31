// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/* *********************************************************************
 * RdWeaponBaseTest
 ***********************************************************************
 *
 * Testing the WeaponBase class
 *
 ***********************************************************************
 */

#include <assert.h>
#include <iostream>

#include "RdWeaponBase.hpp"
#include "RdMacros.hpp"

#define LASER_GUN_MAX_AMMO 10
#define LASER_GUN_DELAY_SHOOT 20
#define LASER_GUN_DELAY_RELOAD 15
#define LASER_GUN_POWER 100

#define MACHINE_GUN_MAX_AMMO 100
#define MACHINE_GUN_DELAY_SHOOT 2
#define MACHINE_GUN_DELAY_RELOAD 25
#define MACHINE_GUN_POWER 10

using namespace rdlib;

void wait( RdWeaponBase& weapon, int frames)
{
    for (int i = 0; i < frames; i++)
        weapon.update();
}

void test_weapon( RdWeaponBase& weapon, int weapon_max_ammo, int weapon_delay_shoot,
                  int weapon_delay_reload, int weapon_power)
{
    RD_INFO("Checking ammo level...\n");
    assert(weapon.getMaxAmmo() == weapon_max_ammo);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking delays...\n");
    assert(weapon.getReloadDelay() == weapon_delay_reload);
    assert(weapon.getShootDelay() == weapon_delay_shoot);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking weapon power...\n");
    assert(weapon.getPower()== weapon_power);

    RD_INFO("Checking shoot()...\n");
    int ammo_before_shooting = weapon.getCurrentAmmo();
    assert(weapon.shoot());
    assert(weapon.getCurrentAmmo() == ammo_before_shooting-1);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking update()...\n");
    wait(weapon, weapon_delay_shoot-1);
    assert(weapon.getShootCurrentDelay()==1);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking shoot before delay time...\n");
    assert(!weapon.shoot());
    wait(weapon, 1);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking shoot with no ammo...\n");
    bool result;
    for (int i = 0; i < weapon_max_ammo; i++)
    {
        result = weapon.shoot();
        wait(weapon, weapon_delay_shoot);
    }
    assert(weapon.getCurrentAmmo()==0);
    assert(!result);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking reload...\n");
    assert(weapon.reload());
    assert(weapon.getCurrentAmmo()==weapon_max_ammo);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking reload before delay...\n");
    for (int i = 0; i < weapon_max_ammo-1; i++)
    {
        weapon.shoot();
        wait(weapon, weapon_delay_shoot);
    }
    weapon.shoot();
    weapon.reload();
    wait(weapon, weapon_delay_reload-1);
    assert(weapon.getReloadCurrentDelay()==1);
    assert(!weapon.reload());
    RD_SUCCESS("[ok]\n");
}

int main(void)
{
    RD_INFO("Testing RdWeaponBase class\n");
    RD_INFO("Creating a laser gun weapon...\n");
    RdWeaponBase laser_gun(0);
    RD_SUCCESS("[ok]\n");

    test_weapon(laser_gun, LASER_GUN_MAX_AMMO, LASER_GUN_DELAY_SHOOT, LASER_GUN_DELAY_RELOAD,
                LASER_GUN_POWER);

    RD_INFO("Creating a laser gun weapon...\n");
    RdWeaponBase machine_gun(1);
    RD_SUCCESS("[ok]\n");

    test_weapon(machine_gun, MACHINE_GUN_MAX_AMMO, MACHINE_GUN_DELAY_SHOOT, MACHINE_GUN_DELAY_RELOAD,
                MACHINE_GUN_POWER);
}
