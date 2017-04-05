// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MOCK_IMAGE_MANAGER_HPP__
#define __RD_MOCK_IMAGE_MANAGER_HPP__

#include <string>

#include <yarp/os/Semaphore.h>

#include "ImageManager.hpp"

namespace rd{

/**
 * @ingroup ImageManagerLib
 *
 * @brief Image input manager based in YARP
 *
 * ImageManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first have to register the manager with the RegisterManager method.
 * Then, we can get the reference to the MockImageManager with ImageManager::getImageManager(), using
 * the corresponding id and access the manager with that reference.
 *
 * Imnage events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the reference to the manager that triggered them to be able to access the image
 *
 */
class MockImageManager : public ImageManager
{
    public:
        virtual bool start();
        virtual bool stop();
        virtual bool isStopped() const;
        virtual bool setEnabled(bool enabled);
        virtual bool isEnabled() const;
        virtual bool configure(std::string parameter, std::string value);
        virtual Image getImage() const;

        /**
         * @brief Register this manager in the ImageManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance)
         */
        static bool RegisterManager();

        //! @brief Destructor. Used to reset the local static reference after destroying this manager
        virtual ~MockImageManager();

        //! @brief String that identifies this manager
        static const std::string id;

        //! @brief Simulate image has been received from device
        bool receiveImage(Image received_image);

    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the getMentalMap() method
         */
        MockImageManager();

        //! @brief Reference to this manager (unique instance)
        static MockImageManager * uniqueInstance;

        //! @brief Semaphore to make the image manipulation thread-safe
        mutable yarp::os::Semaphore semaphore;

        //! @brief Last image received
        Image image;

        bool stopped;
        bool enabled;
};



}

#endif //-- __RD_MOCK_IMAGE_MANAGER_HPP__
