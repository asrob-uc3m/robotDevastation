# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

add_library(ImageLib ImageEventListener.hpp
                     ImageManager.hpp
                     ImageManager.cpp
                     MockImageEventListener.hpp
                     MockImageEventListener.cpp
                     MockImageManager.hpp
                     MockImageManager.cpp
                     ProcessorImageEventListener.hpp
                     ProcessorImageEventListener.cpp
                     YarpImageManager.hpp
                     YarpImageManager.cpp
                     YarpLocalImageManager.hpp
                     YarpLocalImageManager.cpp)

target_include_directories(ImageLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                           ${ZBAR_INCLUDE_DIR})

target_link_libraries(ImageLib PUBLIC YARP::YARP_os
                                      YARP::YARP_dev
                                      YARP::YARP_sig
                                      ${ZBAR_LIBRARIES}
                                      MentalMapLib
                               PRIVATE YARP::YARP_init
                                       UtilsLib)
