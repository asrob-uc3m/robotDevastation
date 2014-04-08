#include "RdBullet.hpp"


RdBullet::RdBullet(int bullet_type, RdVector2d initial_pos, RdVector2d initial_velocity)
{
    if (bullet_type == BULLET_LASER_GUN )
    {
        this->size = 2;
        this->power = 100;

    }
    else if (bullet_type == BULLET_MACHINE_GUN )
    {
        this->size = 1;
        this->power = 10;
    }
    else
    {
        RD_ERROR("Cannot create bullet, specified type does not exist");
        return;
    }

    this->bullet_type = bullet_type;
    this->pos = initial_pos;
    this->velocity = initial_velocity;
}

bool RdBullet::update()
{
    this->pos += this->velocity;
}

bool RdBullet::update(RdVector2d new_pos)
{
    this->pos = new_pos;
}

bool RdBullet::update(RdVector2d new_pos, RdVector2d new_velocity)
{
    this->pos = new_pos;
    this->velocity = new_velocity;
}

int RdBullet::getBulletType()
{
    return bullet_type;
}

int RdBullet::getSize()
{
    return size;
}

int RdBullet::getPower()
{
    return power;
}

RdVector2d RdBullet::getPos()
{
    return pos;
}

RdVector2d RdBullet::getVelocity()
{
    return velocity;
}
