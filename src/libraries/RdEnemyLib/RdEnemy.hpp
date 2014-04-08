// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/* *********************************************************************
 * RdEnemy
 ***********************************************************************
*/

#ifndef __RD_ENEMY_HPP__
#define __RD_ENEMY_HPP__

#include <string>
#include "RdUtils.hpp"
#include "RdBullet.hpp"


namespace rdlib {

class RdEnemy
{
    public:
        RdEnemy(int id, int enemy_type, RdVector2d pos, int width, int height);

        bool updatePos(RdVector2d newPos);
        bool hit(RdBullet bullet);

        static const int ENEMY_HEAD = 0;
        static const int QR_CODE = 1;

        int getId();
        int getPos();
        int getDimensions();
        int getMaxHealth();
        int getCurrentHealth();
        std::string getName();

    private:
        bool existCollision(RdBullet bullet);

        int id;
        RdVector2d pos;
        int width, height;
        int max_health;
        int current_health;
        std::string name;
};
}

#endif
