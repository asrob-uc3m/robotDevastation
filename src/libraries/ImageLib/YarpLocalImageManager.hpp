// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_YARP_LOCAL_IMAGE_MANAGER_HPP__
#define __RD_YARP_LOCAL_IMAGE_MANAGER_HPP__

#include <string>

#include <yarp/os/PortReaderBuffer.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Semaphore.h>
#include <yarp/dev/PolyDriver.h>

#include "ImageManager.hpp"

namespace rd{

/**
 * @ingroup ImageLib
 *
 * @brief Image input manager based in YARP
 *
 * ImageManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first have to register the manager with the RegisterManager() method.
 * Then, we can get the reference to the YarpLocalImageManager with ImageManager::getImageManager(), using
 * the corresponding id and access the manager with that reference.
 *
 * Imnage events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the reference to the manager that triggered them to be able to access the image.
 *
 */
class YarpLocalImageManager : public ImageManager,
                           public yarp::os::TypedReaderCallback<Image>
{
    public:
        virtual bool start();
        virtual bool stop();
        virtual bool isStopped() const;
        virtual bool setEnabled(bool enabled);
        virtual bool configure(const std::string & parameter, const std::string & value);
        virtual Image getImage() const;

        /**
         * @brief Register this manager in the ImageManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance).
         */
        static bool RegisterManager();

        //! @brief Destructor. Used to reset the local static reference after destroying this manager
        virtual ~YarpLocalImageManager();

        //! @brief String that identifies this manager
        static const std::string id;

    protected:
        //! @brief Yarp event for incoming messages
        void onRead(Image& image);

    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the RegisterManager() method.
         */
        YarpLocalImageManager();

        YarpLocalImageManager(const YarpLocalImageManager &);
        YarpLocalImageManager & operator=(const YarpLocalImageManager &);

        //! @brief Reference to this manager (unique instance)
        static YarpLocalImageManager * uniqueInstance;

        //! @brief Semaphore to make the image manipulation thread-safe
        mutable yarp::os::Semaphore semaphore;

        //! @brief Last image received
        Image image;

        //! @brief Yarp port to communicate with the remote camera
        yarp::os::BufferedPort<Image> imagePort;

        //! @brief Name of the local yarp port
        std::string local_port_name;

        //! @brief Name of the camera (remote) yarp port
        std::string remote_port_name;

        bool stopped;
        bool enabled;

        yarp::dev::PolyDriver cameraDevice;

        //! @brief Used to select when several cameras are attached
        int camera_id;
};



}

#endif //-- __RD_YARP_LOCAL_IMAGE_MANAGER_HPP__
