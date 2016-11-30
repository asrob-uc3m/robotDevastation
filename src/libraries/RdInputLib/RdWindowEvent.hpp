// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_WINDOW_EVENT__
#define __RD_WINDOW_EVENT__


namespace rd{

/**
 * @ingroup RdInputLib
 *
 * @brief Abstract class that represents a window event
 *
 */
class RdWindowEvent
{
    public:
        /**
         * @brief Get the identifier for this window event
         * @return An integer type that stores an identifier representing the current window event
         */
        virtual int getEvent() const;

        //! @brief Constant representing a close window action
        static const int WINDOW_CLOSE;

        virtual ~RdWindowEvent() = 0;

    protected:
        RdWindowEvent();

        //! @brief Stores an identifier for the window event
        int event_id;
};

}

#endif //-- __RD_WINDOW_EVENT__
