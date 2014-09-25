// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MENTAL_MAP_EVENT_LISTENER_HPP__
#define __RD_MENTAL_MAP_EVENT_LISTENER_HPP__

#include "RdPlayer.hpp"
#include "RdTarget.hpp"
#include "RdWeapon.hpp"

namespace rd{

class RdMentalMapEventListener
{
    public:
        virtual bool onTargetHit(RdTarget target, RdPlayer player, RdWeapon weapon) = 0;
};

}

#endif //-- __RD_MENTAL_MAP_EVENT_LISTENER_HPP__

