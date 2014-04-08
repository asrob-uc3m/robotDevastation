// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/* *********************************************************************
 * RdBullet
 ***********************************************************************
*/

#ifndef __RD_BULLET_HPP__
#define __RD_BULLET_HPP__

#include "RdUtils.hpp"

class RdBullet
{
public:
    RdBullet(int bullet_type, RdVector2d initial_pos,
             RdVector2d initial_velocity);

    bool update();
    bool update(RdVector2d new_pos);
    bool update(RdVector2d new_pos, RdVector2d new_velocity);

    int getBulletType();
    int getSize();
    int getPower();
    RdVector2d getPos();
    RdVector2d getVelocity();

    static const int BULLET_LASER_GUN = 0;
    static const int BULLET_MACHINE_GUN = 1;

private:
    int bullet_type;
    int size;
    int power;
    RdVector2d pos;
    RdVector2d velocity;

};

#endif
