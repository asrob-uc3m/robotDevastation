
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

add_subdirectory(RdUtilsLib)
add_subdirectory(RdCameraLib)
add_subdirectory(RdImageProcessLib)
add_subdirectory(RdInputLib)
add_subdirectory(RdOutputLib)
add_subdirectory(RdRobotLib)
add_subdirectory(RdManagerLib)
add_subdirectory(RdWeaponLib)
add_subdirectory(RdEnemyLib)
add_subdirectory(RdBulletLib)

