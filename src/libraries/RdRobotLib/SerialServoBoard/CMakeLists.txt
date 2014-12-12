# Copyright: (C) 2014 ASROB@UC3M
# Authors (RD fork): Juan G Victores, David Estevez

# Copyright: (C) 2009 RobotCub Consortium
# Authors: Giacomo Spigler, Paul Fitzpatrick

# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 
IF (COMPILE_DEVICE_LIBRARY)
  YARP_PREPARE_DEVICE(RdSerialServoBoard TYPE SerialServoBoard INCLUDE "SerialServoBoard.h" WRAPPER controlboard)
ENDIF (COMPILE_DEVICE_LIBRARY)

IF (NOT SKIP_RdSerialServoBoard)


  FIND_PACKAGE(YARP REQUIRED)

  INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})
  yarp_add_plugin(RdSerialServoBoard SerialServoBoard.cpp SerialServoBoard.h)
  TARGET_LINK_LIBRARIES(RdSerialServoBoard ${YARP_LIBRARIES})

# Exporting dependencies for RDConfig.cmake quite manually for now...
#set(RD_INCLUDE_DIRS ${RD_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "appended header dirs") # done in parent
set(RD_LIBRARIES ${RD_LIBRARIES} RdSerialServoBoard CACHE INTERNAL "appended libraries") # Not for now as it is a header

ENDIF (NOT SKIP_RdSerialServoBoard)

