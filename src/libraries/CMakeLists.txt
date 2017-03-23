
# Order may be important here
### macros
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        SET(dirlist ${dirlist} ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
FOREACH(subdir ${SUBDIRS})
    set(RD_INCLUDE_DIRS ${RD_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/${subdir} CACHE INTERNAL "appended header dirs")
ENDFOREACH()

add_subdirectory(UtilsLib)
add_subdirectory(MusicLib)
add_subdirectory(MentalMapLib)
add_subdirectory(UserInterfaceLib)
add_subdirectory(InputLib)
add_subdirectory(NetworkLib)
add_subdirectory(RobotLib)
add_subdirectory(ImageLib)
add_subdirectory(StateMachineLib)
add_subdirectory(GameStatesLib)
add_subdirectory(ServerLib)

