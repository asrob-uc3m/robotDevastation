#ifndef __RD_SCREEN_HPP__
#define __RD_SCREEN_HPP__

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
        virtual bool show() = 0;
        virtual ~RdScreen() {}

    protected:
        RdScreen();
};

}

#endif //-- __RD_SCREEN_HPP__
