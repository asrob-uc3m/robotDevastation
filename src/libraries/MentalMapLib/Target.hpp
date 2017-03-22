// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_TARGET_HPP__
#define __RD_TARGET_HPP__

#include "Player.hpp"
#include "Vector2dBase.hpp"

namespace rd{

/**
 * @ingroup RdMentalMapLib
 *
 * @brief Class that represents a target detected. This target is (or should be) associated to a player.
 *
 */
class RdTarget
{
public:
    //! @brief Constructs a default, dummy target. (That is invalid for the game, by the way)
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
    //! @brief Id of the player that is represented by this target
    int player_id;

    //! @brief Center of the box that bounds this target
    RdVector2d pos;

    //! @brief Height and Width of the box that bounds this target
    RdVector2d dimensions;

    //! @brief Quantity representing how sure we are that this target can currently be seen by the user's robot
    int belief;

    static const int MAX_BELIEF = 100;
};
}

#endif //__RD_TARGET_HPP__
