include(YCMEPHelper)

ycm_ep_helper(ASROB_YARP_DEVICES TYPE GIT
              STYLE GITHUB
              REPOSITORY asrob-uc3m/yarp-devices.git
              TAG master)

# Include path to device interface headers.
ExternalProject_Get_Property(ASROB_YARP_DEVICES INSTALL_DIR)
include_directories(${INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR})

# Needed by dependencies on configure time.
if(NOT TARGET ASROB::RobotInterfaces)
    # UNKNOWN breaks on executable targets, use INTERFACE
    add_library(ASROB::RobotInterfaces INTERFACE IMPORTED)
endif()
