#include "RdEnemy.hpp"

rd::RdPlayer *rd::RdEnemy::getPlayer() const
{
    return player;
}

void rd::RdEnemy::setPlayer(RdPlayer *value)
{
    player = value;
}

RdVector2d rd::RdEnemy::getPos() const
{
    return pos;
}

void rd::RdEnemy::setPos(const RdVector2d &value)
{
    pos = value;
}

RdVector2d rd::RdEnemy::getDimensions() const
{
    return dimensions;
}

void rd::RdEnemy::setDimensions(const RdVector2d &value)
{
    dimensions = value;
}


rd::RdEnemy::RdEnemy(rd::RdPlayer *player, RdVector2d pos, RdVector2d dimensions)
{
    this->player = player;
    this->pos = pos;
    this->dimensions = dimensions;
    this->belief = 1;
}
