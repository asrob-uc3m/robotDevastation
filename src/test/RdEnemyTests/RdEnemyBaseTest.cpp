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
#include "RdUtils.hpp"

using namespace rdlib;


int test_enemy( RdEnemy& enemy)
{

}

int main(void)
{
    RD_INFO("Testing RdWeaponBase class\n");
    RD_INFO("Creating a laser gun weapon...\n");
    RdEnemy dummy(0, RdEnemy::ENEMY_HEAD, RdVector2d(200, 200), 40, 40);

    RD_SUCCESS("[ok]\n");

    test_enemy(dummy);
}
