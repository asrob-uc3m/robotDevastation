# Copyright: (C) 2013 Universidad Carlos III de Madrid
# Author: Juan G. Victores
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.6)

prepare_device(emptybot TYPE EmptyBot INCLUDE EmptyBot.hpp WRAPPER controlboard)

IF (NOT SKIP_emptybot)

  find_package(YARP REQUIRED)

  # Include any directories needed for YARP
  include_directories(${YARP_INCLUDE_DIRS})

  yarp_add_plugin(emptybot EmptyBot.cpp IDeviceImpl.cpp IEncoderImpl.cpp IPositionImpl.cpp IVelocityImpl.cpp )
  TARGET_LINK_LIBRARIES(emptybot ${YARP_LIBRARIES})
  #install(TARGETS emptybot DESTINATION lib)

ENDIF (NOT SKIP_emptybot)

