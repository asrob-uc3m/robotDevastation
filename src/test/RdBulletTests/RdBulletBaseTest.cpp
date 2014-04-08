// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/* *********************************************************************
 * RdBulletBaseTest
 ***********************************************************************
 *
 * Testing the Bullet class
 *
 ***********************************************************************
 */

#include <assert.h>
#include <iostream>

#include "RdEnemy.hpp"
#include "RdUtils.hpp"

#define LASER_GUN_SIZE 2
#define LASER_GUN_POWER 100

#define MACHINE_GUN_SIZE 1
#define MACHINE_GUN_POWER 10


using namespace rdlib;


int test_enemy( RdBullet& bullet, int bullet_type,
                 RdVector2d initial_pos, RdVector2d initial_velocity,
                 int bullet_size, int bullet_power)
{
    RD_INFO("Checking bullet type...\n");
    assert(bullet.getBulletType() == bullet_type);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking initial pos...\n");
    assert(bullet.getPos() == initial_pos);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking initial velocity...\n");
    assert(bullet.getVelocity() == initial_velocity);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking bullet size...\n");
    assert(bullet.getSize() == bullet_size);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking bullet power...\n");
    assert(bullet.getPower() == bullet_power);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking updates (standard)...\n");
    RdVector2d init_pos = bullet.getPos();
    RdVector2d init_vel = bullet.getVelocity();
    RdVector2d target_pos = RdVector2d( init_pos.x + 10*init_vel.x,
                                        init_pos.y + 10*init_vel.y);

    for (int i = 0; i < 10; i++)
    {
        assert(bullet.update() == true);
    }
    assert(bullet.getPos() == target_pos);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking updates (setpos)...\n");
    RdVector2d target_pos_2 = RdVector2d(40, 40);
    assert(bullet.update(target_pos_2) == true);
    assert(bullet.getPos() == target_pos_2);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking updates (setpos&vel)...\n");
    RdVector2d target_pos_3 = RdVector2d(40, 40);
    RdVector2d target_vel = RdVector2d(10, 10);
    assert(bullet.update(target_pos_3, target_vel) == true);
    assert(bullet.getPos() == target_pos_3);
    assert(bullet.getVelocity() == target_vel );
    RD_SUCCESS("[ok]\n");
}

int main(void)
{
    RD_INFO("Testing RdBullet class\n");
    RD_INFO("Creating a laser gun bullet...\n");
    RdBullet laser_bullet( RdBullet::BULLET_LASER_GUN, RdVector2d(0, 0),
                    RdVector2d(20, 10));

    RD_SUCCESS("[ok]\n");

    test_enemy(laser_bullet, RdBullet::BULLET_LASER_GUN, RdVector2d(0, 0),
                RdVector2d(20, 10), LASER_GUN_SIZE, LASER_GUN_POWER);

    RD_INFO("Creating a machine gun bullet...\n");
    RdBullet machine_gun_bullet( RdBullet::BULLET_MACHINE_GUN, RdVector2d(10, 10),
                    RdVector2d(-1, 0));

    RD_SUCCESS("[ok]\n");

    test_enemy(machine_gun_bullet, RdBullet::BULLET_MACHINE_GUN, RdVector2d(10, 10),
                RdVector2d(-1, 0), MACHINE_GUN_SIZE, MACHINE_GUN_POWER);

}
