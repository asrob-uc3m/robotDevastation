# Thanks: https://svn.engsas.de/meganizer/trunk/cmake/modules/FindZBar0.cmake
#### - Try to find ZBar (with and without Qt)  #-- Comment out ZBAR_QT
# - Try to find ZBar
# Once done this will define
#
#  ZBAR_FOUND - system has ZBar
####  ZBAR_QT_FOUND - system has ZBar with Qt  #-- Comment out ZBAR_QT
#  ZBAR_LIBRARY_DIR - the QCA2 top level directory
#  ZBAR_INCLUDE_DIR - the QCA2 include directory
#  ZBAR_LIBRARIES - the libraries needed to ZBar
####  ZBAR_QT_LIBRARIES - the libraries needed to ZBar Qt #-- Comment out ZBAR_QT
#
# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls

# Copyright (C) 2010  EngSaS - Engineering Solutions and Services Langenbach. All rights reserved.

IF (ZBAR_INCLUDE_DIR AND ZBAR_LIBRARIES)

  # in cache already
  SET(ZBAR_FOUND TRUE)

ELSE (ZBAR_INCLUDE_DIR AND ZBAR_LIBRARIES)


  IF (WIN32)
		FIND_PATH(ZBAR_LIBRARY_DIR
							WIN32_DEBUG_POSTFIX d
							NAMES "bin/libzbar-0.dll"
              HINTS "C:/Programme/" "C:/Program Files"
							PATH_SUFFIXES ZBar
              )
		FIND_LIBRARY(ZBAR_LIBRARIES NAMES libzbar-0.dll HINTS ${ZBAR_LIBRARY_DIR}/bin)
		FIND_PATH(ZBAR_INCLUDE_DIR NAMES Decoder.h HINTS ${ZBAR_LIBRARY_DIR} PATH_SUFFIXES include/zbar)
	ELSE(WIN32)
    FIND_PACKAGE(PkgConfig)
#     pkg_check_modules(PC_QCA2 QUIET qca2)
    pkg_check_modules(PC_ZBAR zbar)
    SET(ZBAR_DEFINITIONS ${PC_ZBAR_CFLAGS_OTHER})
		FIND_LIBRARY(ZBAR_LIBRARIES
                  WIN32_DEBUG_POSTFIX d
                  NAMES zbar
                  HINTS ${PC_ZBAR_LIBDIR} ${PC_ZBAR_LIBRARY_DIRS}
                  )
		FIND_PATH(ZBAR_INCLUDE_DIR Decoder.h
            	HINTS ${PC_ZBAR_INCLUDEDIR} ${PC_ZBAR_INCLUDE_DIRS}
            	PATH_SUFFIXES zbar)
  ENDIF (WIN32)

  INCLUDE(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(ZBAR  DEFAULT_MSG  ZBAR_LIBRARIES ZBAR_INCLUDE_DIR)

# mark_as_advanced(QCA2_INCLUDE_DIR QCA2_LIBRARIES)

ENDIF (ZBAR_INCLUDE_DIR AND ZBAR_LIBRARIES)

#-- Comment out ZBAR_QT
###IF (ZBAR_QT_LIBRARIES)
###
###  # in cache already
###  SET(ZBAR_QT_FOUND TRUE)
###
###ELSE (ZBAR_QT_LIBRARIES)
###
###  IF (WIN32)
###		FIND_LIBRARY(ZBAR_QT_LIBRARIES NAMES libzbarqt0.dll HINTS ${ZBAR_LIBRARY_DIR}/bin)
###		FIND_PATH(ZBAR_QT_INCLUDE_DIR NAMES QZBar.h HINTS ${ZBAR_INCLUDE_DIR})
###	ELSE(WIN32)
###		FIND_PACKAGE(PkgConfig)
####     pkg_check_modules(PC_QCA2 QUIET qca2)
###    pkg_check_modules(PC_ZBAR_QT zbar-qt)
###    SET(ZBAR_QT_DEFINITIONS ${PC_ZBAR_QT_CFLAGS_OTHER})
###		FIND_LIBRARY(ZBAR_QT_LIBRARIES
###                  WIN32_DEBUG_POSTFIX d
###                  NAMES zbarqt
###                  HINTS ${ZBAR_LIBRARY_DIR}
###                  )
###		FIND_PATH(ZBAR_QT_INCLUDE_DIR QZBar.h
###							HINTS ${PC_ZBAR_QT_INCLUDEDIR} ${PC_ZBAR_QT_INCLUDE_DIRS} ${ZBAR_INCLUDE_DIR}
###							PATH_SUFFIXES zbar)
###  ENDIF (WIN32)
###
###	SET(ZBAR_QT_INCLUDE_DIR ${ZBAR_INCLUDE_DIR})
###
###	INCLUDE(FindPackageHandleStandardArgs)
###  find_package_handle_standard_args(ZBAR_QT  DEFAULT_MSG  ZBAR_QT_LIBRARIES ZBAR_QT_INCLUDE_DIR)
###	# QT versions get the not qt variables appended, so that only qt stuff must be included
###	# and the other needed stuff is automatically included to
###	#SET(ZBAR_QT_INCLUDE_DIR "${ZBAR_QT_INCLUDE_DIR} ${ZBAR_INCLUDE_DIR}")
###	SET(ZBAR_QT_LIBRARIES ${ZBAR_QT_LIBRARIES} ${ZBAR_LIBRARIES})
###
###ENDIF (ZBAR_QT_LIBRARIES)
