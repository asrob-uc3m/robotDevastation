// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MOUSE_MANAGER_HPP__
#define __RD_MOUSE_MANAGER_HPP__

#include "RdUtils.hpp"
#include "RdInputEventListener.hpp"

#include <vector>
#include "SDL/SDL.h"

namespace rd{

class RdMouseManager
{
    public:
        RdMouseManager();
        ~RdMouseManager();

        bool start();

        bool setInputEventListeners( std::vector<RdInputEventListener *> * listeners );

    protected:
        bool update();

    private:
        std::vector<RdInputEventListener *> * listeners;

};

}

#endif //-- __RD_MOUSE_MANAGER_HPP__
