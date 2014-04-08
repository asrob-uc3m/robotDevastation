// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/* *********************************************************************
 * RdEnemyBaseTest
 ***********************************************************************
 *
 * Testing the EnemyBase class
 *
 ***********************************************************************
 */

#include <assert.h>
#include <iostream>

#include "RdEnemy.hpp"
#include "RdBullet.hpp"
#include "RdUtils.hpp"

using namespace rdlib;

const std::string HEAD_NAME = "Head";
#define HEAD_MAX_HEALTH 500

const std::string QR_NAME = "QR";
#define QR_MAX_HEALTH 500

void test_enemy( RdEnemy& enemy, int id,
                 RdVector2d initial_pos, int width, int height,
                 std::string name, int max_health )
{
    RD_INFO("Checking id...\n");
    assert(enemy.getId() == id);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking initial pos...\n");
    assert(enemy.getPos() == initial_pos);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking width...\n");
    assert(enemy.getWidth() == width);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking height...\n");
    assert(enemy.getHeight() == height);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking name...\n");
    assert(enemy.getName() == name);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking max health...\n");
    assert(enemy.getMaxHealth() == max_health);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Checking initial health...\n");
    assert(enemy.getCurrentHealth() == max_health);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Testing being hit decreases health...\n");
    RdBullet bullet(RdBullet::BULLET_LASER_GUN, initial_pos, RdVector2d(0,0));
    int power = bullet.getPower();
    assert(enemy.hit(bullet) == true);
    assert(enemy.getCurrentHealth() == max_health-power);
    RD_SUCCESS("[ok]\n");

    RD_INFO("Testing not being hit doesn't decrease health...\n");
    RdVector2d outside_target = RdVector2d( initial_pos.x + enemy.getWidth(),
                                            initial_pos.y + enemy.getHeight());
    RdBullet bullet2(RdBullet::BULLET_LASER_GUN, outside_target, RdVector2d(0,0));
    int power2 = bullet2.getPower();
    int health = enemy.getCurrentHealth();
    assert(enemy.hit(bullet2) == false);
    assert(enemy.getCurrentHealth() == health);
    RD_SUCCESS("[ok]\n");

}

int main(void)
{
    RD_INFO("Testing RdWeaponBase class\n");
    RD_INFO("Creating a laser gun weapon...\n");
    RdEnemy dummy(0, RdEnemy::ENEMY_HEAD, RdVector2d(200, 200), 40, 40);

    RD_SUCCESS("[ok]\n");

    test_enemy(dummy, 0, RdVector2d(200, 200), 40, 40, HEAD_NAME, HEAD_MAX_HEALTH);
}
