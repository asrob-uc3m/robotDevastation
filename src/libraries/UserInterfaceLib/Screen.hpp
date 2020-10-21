// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_SCREEN_HPP__
#define __RD_SCREEN_HPP__

#include <string>
#include <vector>
#include "ImageManager.hpp"
#include "Player.hpp"
#include "Target.hpp"
#include "Weapon.hpp"

namespace rd{

/**
 * @ingroup UserInterfaceLib
 *
 * @brief A User Interface.
 *
 */

class Screen
{
    public:
        virtual bool init() = 0;
        virtual bool cleanup() = 0;
        virtual bool drawScreen(void *screen) = 0;
        virtual bool update(const std::string & parameter, const std::string & value);
        virtual bool update(const std::string & parameter, const Image & value); //-- Required by GameScreen and DeadScreen
        virtual bool update(const std::string & parameter, const Player & value); //-- Required by GameScreen
        virtual bool update(const std::string & parameter, const std::vector<Player> & value); //-- Required by GameScreen
        virtual bool update(const std::string & parameter, const std::vector<Target> & value); //-- Required by GameScreen
        virtual bool update(const std::string & parameter, const Weapon & value); //-- Required by GameScreen
        virtual int getHeight() const;
        virtual int getWidth() const;
        virtual ~Screen() {}

    protected:
        Screen();

        int h, w; //-- Screen size
};

}

#endif //-- __RD_SCREEN_HPP__
