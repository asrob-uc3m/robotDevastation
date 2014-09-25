# Copyright: (C) 2013 Universidad Carlos III de Madrid
# Author: Juan G. Victores
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.6)

prepare_device(pwmbot TYPE PwmBot INCLUDE PwmBot.hpp WRAPPER controlboard)

IF (NOT SKIP_pwmbot)

  find_package(YARP REQUIRED)

  # Include any directories needed for YARP
  include_directories(${YARP_INCLUDE_DIRS})

  yarp_add_plugin(pwmbot PwmBot.cpp IDeviceImpl.cpp IEncoderImpl.cpp IPositionImpl.cpp IVelocityImpl.cpp )
  TARGET_LINK_LIBRARIES(pwmbot ${YARP_LIBRARIES})
  #install(TARGETS pwmbot DESTINATION lib)

ENDIF (NOT SKIP_pwmbot)

