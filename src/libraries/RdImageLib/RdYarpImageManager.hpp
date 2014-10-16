// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_YARP_IMAGE_MANAGER_HPP__
#define __RD_YARP_IMAGE_MANAGER_HPP__


#include <vector>
#include <map>
#include <string>

#include "RdUtils.hpp"
#include "RdImageManager.hpp"
#include "RdImageEventListener.hpp"
#include <yarp/sig/Image.h>


namespace rd{

/**
 * @ingroup RdImageManagerLib
 *
 * @brief Image input manager based in YARP
 *
 * RdImageManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first have to register the manager with the RegisterManager method.
 * Then, we can get the reference to the RdYarpImageManager with  RdImageManager::getImageManager(), using
 * the corresponding id and access the manager with that reference.
 *
 * Imnage events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the reference to the manager that triggered them to be able to access the image
 *
 */
class RdYarpImageManager : public RdImageManager
{
    public:
        virtual bool start();
        virtual bool stop();
        virtual bool configure(std::string parameter, std::string value);
        virtual RdImage getImage();

        /**
         * @brief Register this manager in the RdImageManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance)
         */
        static bool RegisterManager();

        //! @brief Destructor. Used to reset the local static reference after destroying this manager
        ~RdYarpImageManager();

        //! @brief String that identifies this manager
        static const std::string id;

    private:
        //! @brief Reference to this manager (unique instance)
        static RdYarpImageManager * uniqueInstance;
};



}

#endif //-- __RD_YARP_IMAGE_MANAGER_HPP__
