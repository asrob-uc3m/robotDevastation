// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_IMAGE_MANAGER_HPP__
#define __RD_IMAGE_MANAGER_HPP__


#include <vector>
#include <map>
#include <string>

#include "RdUtils.hpp"
#include "RdImageEventListener.hpp"
#include <yarp/sig/Image.h>


namespace rd{

typedef yarp::sig::ImageOf<yarp::sig::PixelRgb> RdImage;

class RdImageEventListener; //-- Required to avoid recurrent loops in dependencies

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdImageLib
 *
 * @brief The RdImageLib library contains classes related to image input (from cameras, etc)
 */

/**
 * @ingroup RdImageLib
 *
 * @brief Generic image input manager
 *
 * RdImageManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first register the needed subclasses of this class. This way, we can later get the reference
 * to the RdImageManager with getImageManager and the name of the desired registered subclass. Later calls
 * to getImageManager() will return that selected subclass.
 *
 * When the program finishes, the RdImageManager can be deallocated using destroyImageManager().
 *
 * Imnage events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the reference to the manager that triggered them to be able to access the image
 *
 */
class RdImageManager
{
    public:
        //------------------------------ Construction & destruction ---------------------------------------------------//
        /**
         * @brief Get a reference to the RdImageManager
         * @return By default, if no id is specified, this will return a reference to the first
         * RdImageManager that it can find in the registry, or NULL if no RdImageManager was registered.
         */
        static RdImageManager *getImageManager();

        /**
         * @brief Get a reference to the RdImageManager
         * @return The RdImageManager registered with the given id, NULL if the id is not found in
         * the registry.
         */
        static RdImageManager * getImageManager(std::string id);

        //! @brief Deallocate all the registered ImageManager
        static bool destroyImageManager();

        virtual ~RdImageManager();


        //------------------------------ Manager Startup & Halting ----------------------------------------------------//
        /**
         * @brief Start to capture images
         *
         * This function is supposed to be called after RdImageManager configuration.
         */
        virtual bool start() = 0;

        //! @brief Stop capturing images
        virtual bool stop() = 0;

        //! @brief True if the manager is not active
        virtual bool isStopped() = 0;

        //------------------------------- Read image ------------------------------------------------------------------//
        //! @brief Return the last received image
        virtual RdImage getImage() = 0;


        //------------------------------ Configuration & Listeners ----------------------------------------------------//
        //! @brief Adds a RdImageEventListener to the list of observers to be notified of events
        bool addImageEventListener( RdImageEventListener * listener );
        //! @brief Unregisters all the RdImageEventListener stored
        bool removeImageEventListeners();

        //! @brief Configures a parameter with a value
        virtual bool configure(std::string parameter, std::string value);


    protected:
        /**
         * @brief This function allows subclasses to install their unique instances in the singleton register to be
         * selected later by the user
         */
        static bool Register( RdImageManager * manager, std::string id);

        //! @brief Observers registered to be notified of image events
        std::vector<RdImageEventListener *> listeners;

    private:
        //! \brief Stores the unique instance of the RdInputManager
        static RdImageManager * imageManagerInstance;

        //! \brief Stores the id of the current unique instance used
        static std::string currentId;

        //! \brief Stores all the RdImageManager that have been registered
        static std::map< std::string, RdImageManager * > imageManagerRegistry;


};

}

#endif //-- __RD_IMAGE_MANAGER_HPP__
