add_subdirectory(robotDevastation)
add_subdirectory(rdServer)

# Organize targets in folders (Visual Studio)
set_target_properties(rdServer
                      robotDevastation
                      PROPERTIES FOLDER Programs)
