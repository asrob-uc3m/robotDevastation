// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_WINDOW_EVENT__
#define __RD_WINDOW_EVENT__


namespace rd{

/**
 * @ingroup InputLib
 *
 * @brief Class that represents a window event
 *
 * This class must not be inherited from. Use factory classes and functions to perform custom
 * initialization (see rd::SDLEventFactory).
 */
class WindowEvent
{
    public:
        WindowEvent(int event_id);

        /**
         * @brief Get the identifier for this window event
         * @return An integer type that stores an identifier representing the current window event
         */
        int getEvent() const;

        //! @brief Constant representing an unimplemented or unrecognized window action
        static const int WINDOW_UNKNOWN;
        //! @brief Constant representing a close window action
        static const int WINDOW_CLOSE;

    private:
        WindowEvent();

        //! @brief Stores an identifier for the window event
        int event_id;
};

}

#endif //-- __RD_WINDOW_EVENT__
