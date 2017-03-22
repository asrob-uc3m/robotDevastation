#include "Target.hpp"

rd::RdTarget::RdTarget()
{
    this->player_id = -1;
    this->pos = RdVector2d(-1, -1);
    this->dimensions = RdVector2d(-1, -1);
    this->belief = -1;
}

rd::RdTarget::RdTarget(int player_id, RdVector2d pos, RdVector2d dimensions)
{
    this->player_id = player_id;
    this->pos = pos;
    this->dimensions = dimensions;
    this->belief = MAX_BELIEF;
}

int rd::RdTarget::getPlayerId()
{
    return player_id;
}

void rd::RdTarget::setPlayerId(int id)
{
    player_id = id;
}

RdVector2d rd::RdTarget::getPos()
{
    return pos;
}

void rd::RdTarget::setPos(const RdVector2d &pos)
{
    this->pos = pos;
}

RdVector2d rd::RdTarget::getDimensions()
{
    return dimensions;
}

void rd::RdTarget::setDimensions(const RdVector2d &dimensions)
{
   this->dimensions = dimensions;
}

int rd::RdTarget::getBelief()
{
    return belief;
}

bool rd::RdTarget::reduceBelief(int amount)
{
    belief -= amount;

    if ( belief <=0 )
    {
        belief = 0;
        return false;
    }

    return true;
}

bool rd::RdTarget::resetBelief()
{
    belief = MAX_BELIEF;
    return true;
}

