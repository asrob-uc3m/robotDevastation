#ifndef __RD_SCREEN_HPP__
#define __RD_SCREEN_HPP__

#include <string>
#include "RdImageManager.hpp"

namespace rd{

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdUserInterfaceLib
 *
 * @brief The RdUserInterfaceLib library contains the \ref RdGameScreen class.
 */

/**
 * @ingroup RdUserInterfaceLib
 *
 * @brief A User Interface.
 *
 */

class RdScreen
{
    public:
        virtual bool init() = 0;
        virtual bool cleanup() = 0;
        virtual bool show() = 0;
        virtual bool drawScreen(void *screen); //-- Temporary definition as not pure virtual (to avoid breaking the whole inheritance tree while migrating API)
        virtual bool update(std::string parameter, std::string value);
        virtual bool update(std::string parameter, RdImage value);
        virtual ~RdScreen() {}

        int h, w; //-- Screen size

    protected:
        RdScreen();
};

}

#endif //-- __RD_SCREEN_HPP__
