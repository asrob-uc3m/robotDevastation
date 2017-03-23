#include "Target.hpp"

rd::Target::Target()
{
    this->player_id = -1;
    this->pos = Vector2d(-1, -1);
    this->dimensions = Vector2d(-1, -1);
    this->belief = -1;
}

rd::Target::Target(int player_id, Vector2d pos, Vector2d dimensions)
{
    this->player_id = player_id;
    this->pos = pos;
    this->dimensions = dimensions;
    this->belief = MAX_BELIEF;
}

int rd::Target::getPlayerId()
{
    return player_id;
}

void rd::Target::setPlayerId(int id)
{
    player_id = id;
}

Vector2d rd::Target::getPos()
{
    return pos;
}

void rd::Target::setPos(const Vector2d &pos)
{
    this->pos = pos;
}

Vector2d rd::Target::getDimensions()
{
    return dimensions;
}

void rd::Target::setDimensions(const Vector2d &dimensions)
{
   this->dimensions = dimensions;
}

int rd::Target::getBelief()
{
    return belief;
}

bool rd::Target::reduceBelief(int amount)
{
    belief -= amount;

    if ( belief <=0 )
    {
        belief = 0;
        return false;
    }

    return true;
}

bool rd::Target::resetBelief()
{
    belief = MAX_BELIEF;
    return true;
}

