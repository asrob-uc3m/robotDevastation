#include "RdEnemy.hpp"


rdlib::RdEnemy::RdEnemy(int id, int enemy_type, RdVector2d pos, int width, int height)
{
    if ( enemy_type == ENEMY_HEAD )
    {
        max_health = 500;
        name = "Head";
    }
    else if (enemy_type == QR_CODE )
    {
        max_health == 500;
        name = "QR";
    }
    else
    {
        RD_ERROR("Cannot create weapon, specified type does not exist");
        return;
    }

    this->id = id;
    this->pos = pos;
    this->width = width;
    this->height = height;
    this->current_health = max_health;

}

bool rdlib::RdEnemy::updatePos(RdVector2d newPos)
{
    pos = newPos;
}

bool rdlib::RdEnemy::hit(RdBullet bullet)
{
    if (existCollision(bullet))
    {
        if (current_health != 0)
        {
            current_health -= bullet.getPower();
            RD_SUCCESS("Enemy hit!\n");
        }
        return true;
    }
    else
        return false;
}

bool rdlib::RdEnemy::existCollision(RdBullet bullet)
{
    int enemy_start_x = pos.x - width/2;
    int enemy_start_y = pos.y - width/2;
    int enemy_end_x = pos.x + width/2;
    int enemy_end_y = pos.y + width/2;

    int bullet_start_x = bullet.getPos().x - bullet.getSize() / 2;
    int bullet_start_y = bullet.getPos().y - bullet.getSize() / 2;
    int bullet_end_x = bullet.getPos().x + bullet.getSize() / 2;
    int bullet_end_y = bullet.getPos().y + bullet.getSize() / 2;

    if ( (bullet_start_x > enemy_start_x && bullet_start_x < enemy_end_x)
      || (bullet_end_x > enemy_start_x && bullet_end_x < enemy_end_x) )
    {
        if ( (bullet_start_y > enemy_start_y && bullet_start_y < enemy_end_y)
             || (bullet_end_y > enemy_start_y && bullet_end_y < enemy_end_y) )
        {
            return true;
        }
    }
    return false;
}
