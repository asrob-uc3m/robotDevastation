# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

add_subdirectory(GameStatesLib)
add_subdirectory(ImageLib)
add_subdirectory(InputLib)
add_subdirectory(MentalMapLib)
add_subdirectory(MusicLib)
add_subdirectory(NetworkLib)
add_subdirectory(RobotLib)
add_subdirectory(ServerLib)
add_subdirectory(StateMachineLib)
add_subdirectory(UserInterfaceLib)
add_subdirectory(UtilsLib)

# Organize targets in folders (Visual Studio)
set_target_properties(GameStatesLib
                      ImageLib
                      InputLib
                      MentalMapLib
                      MusicLib
                      NetworkLib
                      RobotLib
                      ServerLib
                      StateMachineLib
                      UserInterfaceLib
                      UtilsLib
                      PROPERTIES FOLDER Libraries)
