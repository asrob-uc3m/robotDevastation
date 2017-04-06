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
 * @ingroup rd_libraries
 *
 * \defgroup UserInterfaceLib
 *
 * @brief The UserInterfaceLib library contains the \ref GameScreen class.
 */

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
        virtual bool update(std::string parameter, std::string value);
        virtual bool update(std::string parameter, Image value); //-- Required by GameScreen and DeadScreen
        virtual bool update(std::string parameter, Player value); //-- Required by GameScreen
        virtual bool update(std::string parameter, std::vector<Player> value); //-- Required by GameScreen
        virtual bool update(std::string parameter, std::vector<Target> value); //-- Required by GameScreen
        virtual bool update(std::string parameter, Weapon value); //-- Required by GameScreen
        virtual int getHeight() const;
        virtual int getWidth() const;
        virtual ~Screen() {}

    protected:
        Screen();

        int h, w; //-- Screen size
};

}

#endif //-- __RD_SCREEN_HPP__
