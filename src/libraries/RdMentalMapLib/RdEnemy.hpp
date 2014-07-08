// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_ENEMY_HPP__
#define __RD_ENEMY_HPP__

#include "RdPlayer.hpp"
#include "RdVector2dBase.hpp"

namespace rd{

class RdEnemy
{
public:
    RdEnemy( RdPlayer* player, RdVector2d pos, RdVector2d dimensions );

    RdPlayer *getPlayer() const;
    void setPlayer(RdPlayer *value);

    RdVector2d getPos() const;
    void setPos(const RdVector2d &value);

    RdVector2d getDimensions() const;
    void setDimensions(const RdVector2d &value);

private:
    RdPlayer * player;
    RdVector2d pos;
    RdVector2d dimensions;
    float belief;
};
}

#endif //__RD_ENEMY_HPP__
