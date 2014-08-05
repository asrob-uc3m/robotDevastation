#include "RdEnemy.hpp"

rd::RdEnemy::RdEnemy()
{
    this->player_id = -1;
    this->pos = RdVector2d(-1, -1);
    this->dimensions = RdVector2d(-1, -1);
    this->belief = -1;
}

rd::RdEnemy::RdEnemy(int player_id, RdVector2d pos, RdVector2d dimensions)
{
    this->player_id = player_id;
    this->pos = pos;
    this->dimensions = dimensions;
    this->belief = MAX_BELIEF;
}

int rd::RdEnemy::getPlayerId()
{
    return player_id;
}

void rd::RdEnemy::setPlayerId(int id)
{
    player_id = id;
}

RdVector2d rd::RdEnemy::getPos()
{
    return pos;
}

void rd::RdEnemy::setPos(const RdVector2d &pos)
{
    this->pos = pos;
}

RdVector2d rd::RdEnemy::getDimensions()
{
    return dimensions;
}

void rd::RdEnemy::setDimensions(const RdVector2d &dimensions)
{
   this->dimensions = dimensions;
}

int rd::RdEnemy::getBelief()
{
    return belief;
}

bool rd::RdEnemy::reduceBelief(int amount)
{
    belief -= amount;

    if ( belief <=0 )
    {
        belief = 0;
        return false;
    }

    return true;
}

bool rd::RdEnemy::resetBelief()
{
    belief = MAX_BELIEF;
    return true;
}

