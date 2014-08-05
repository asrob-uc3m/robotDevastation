// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_ENEMY_HPP__
#define __RD_ENEMY_HPP__

#include "RdPlayer.hpp"
#include "RdVector2dBase.hpp"

namespace rd{

class RdEnemy
{
public:
    RdEnemy();
    RdEnemy( int player_id, RdVector2d pos, RdVector2d dimensions );

    int getPlayerId();
    void setPlayerId(int id);

    RdVector2d getPos();
    void setPos(const RdVector2d &pos);

    RdVector2d getDimensions();
    void setDimensions(const RdVector2d &dimensions);

    int getBelief();

private:
    int player_id;
    RdVector2d pos;
    RdVector2d dimensions;
    int belief;
};
}

#endif //__RD_ENEMY_HPP__
