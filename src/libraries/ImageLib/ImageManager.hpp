// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_IMAGE_MANAGER_HPP__
#define __RD_IMAGE_MANAGER_HPP__

#include <vector>
#include <map>
#include <string>

#include <yarp/sig/Image.h>

#include "ImageEventListener.hpp"

namespace rd{

typedef yarp::sig::ImageOf<yarp::sig::PixelRgb> Image;

class ImageEventListener; //-- Required to avoid recurrent loops in dependencies

/**
 * @ingroup rd_libraries
 *
 * \defgroup ImageLib
 *
 * @brief The ImageLib library contains classes related to image input (from cameras, etc)
 */

/**
 * @ingroup ImageLib
 *
 * @brief Generic image input manager
 *
 * ImageManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first register the needed subclasses of this class. This way, we can later get the reference
 * to the ImageManager with getImageManager and the name of the desired registered subclass. Later calls
 * to getImageManager() will return that selected subclass.
 *
 * When the program finishes, the ImageManager can be deallocated using destroyImageManager().
 *
 * Imnage events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the reference to the manager that triggered them to be able to access the image
 *
 */
class ImageManager
{
    public:
        //------------------------------ Construction & destruction ---------------------------------------------------//
        /**
         * @brief Get a reference to the ImageManager
         * @return By default, if no id is specified, this will return a reference to the first
         * ImageManager that it can find in the registry, or NULL if no ImageManager was registered.
         */
        static ImageManager *getImageManager();

        /**
         * @brief Get a reference to the ImageManager
         * @return The ImageManager registered with the given id, NULL if the id is not found in
         * the registry.
         */
        static ImageManager * getImageManager(const std::string & id);

        //! @brief Deallocate all the registered ImageManager
        static bool destroyImageManager();

        virtual ~ImageManager();


        //------------------------------ Manager Startup & Halting ----------------------------------------------------//
        /**
         * @brief Start to capture images
         *
         * This function is supposed to be called after ImageManager configuration.
         */
        virtual bool start() = 0;

        //! @brief Stop capturing images
        virtual bool stop() = 0;

        //! @brief True if the manager is not active
        virtual bool isStopped() const = 0;

        //! @brief Enables/disables notifications to listeners
        virtual bool setEnabled(bool enabled) = 0;

        //------------------------------- Read image ------------------------------------------------------------------//
        //! @brief Return the last received image
        virtual Image getImage() const = 0;


        //------------------------------ Configuration & Listeners ----------------------------------------------------//
        //! @brief Adds a ImageEventListener to the list of observers to be notified of events
        bool addImageEventListener( ImageEventListener * listener );
        //! @brief Unregisters all the ImageEventListener stored
        bool removeImageEventListeners();

        //! @brief Configures a parameter with a value
        virtual bool configure(const std::string & parameter, const std::string & value);


    protected:
        /**
         * @brief This function allows subclasses to install their unique instances in the singleton register to be
         * selected later by the user
         */
        static bool Register( ImageManager * manager, const std::string & id);

        //! @brief Observers registered to be notified of image events
        std::vector<ImageEventListener *> listeners;

    private:
        //! \brief Stores the unique instance of the InputManager
        static ImageManager * imageManagerInstance;

        //! \brief Stores the id of the current unique instance used
        static std::string currentId;

        //! \brief Stores all the ImageManager that have been registered
        static std::map< std::string, ImageManager * > imageManagerRegistry;


};

}

#endif //-- __RD_IMAGE_MANAGER_HPP__
