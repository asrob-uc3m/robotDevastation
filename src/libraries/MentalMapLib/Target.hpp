// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_TARGET_HPP__
#define __RD_TARGET_HPP__

#include "Vector2dBase.hpp"

namespace rd{

/**
 * @ingroup MentalMapLib
 *
 * @brief Class that represents a target detected. This target is (or should be) associated to a player.
 *
 */
class Target
{
public:
    //! @brief Constructs a default, dummy target. (That is invalid for the game, by the way)
    Target();
    Target( int player_id, const Vector2d &pos, const Vector2d &dimensions);

    int getPlayerId() const;
    void setPlayerId(int id);

    const Vector2d & getPos() const;
    void setPos(const Vector2d &pos);

    const Vector2d & getDimensions() const;
    void setDimensions(const Vector2d &dimensions);

    int getBelief() const;
    bool reduceBelief(int amount);
    bool resetBelief();

private:
    //! @brief Id of the player that is represented by this target
    int player_id;

    //! @brief Center of the box that bounds this target
    Vector2d pos;

    //! @brief Height and Width of the box that bounds this target
    Vector2d dimensions;

    //! @brief Quantity representing how sure we are that this target can currently be seen by the user's robot
    int belief;

    static const int MAX_BELIEF = 100;
};
}

#endif //__RD_TARGET_HPP__
