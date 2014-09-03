// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_ENEMY_HPP__
#define __RD_ENEMY_HPP__

#include "RdPlayer.hpp"
#include "RdVector2dBase.hpp"

namespace rd{

class RdTarget
{
public:
    RdTarget();
    RdTarget( int player_id, RdVector2d pos, RdVector2d dimensions);

    int getPlayerId();
    void setPlayerId(int id);

    RdVector2d getPos();
    void setPos(const RdVector2d &pos);

    RdVector2d getDimensions();
    void setDimensions(const RdVector2d &dimensions);

    int getBelief();
    bool reduceBelief(int amount);
    bool resetBelief();

private:
    int player_id;
    RdVector2d pos;
    RdVector2d dimensions;
    int belief;

    static const int MAX_BELIEF = 100;
};
}

#endif //__RD_ENEMY_HPP__
