
cmake_minimum_required(VERSION 2.8.7)

IF (ENABLE_rdServer)

set(KEYWORD "rdServer")

# Find YARP.  Point the YARP_DIR environment variable at your build.
find_package(YARP REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${YARP_MODULE_PATH})

# Search for source code.
file(GLOB folder_source *.cpp *.cc *.c)
file(GLOB folder_header *.h)
source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})

# Automatically add include directories if needed.
foreach(header_file ${folder_header})
  get_filename_component(p ${header_file} PATH)
  include_directories(${p})
endforeach(header_file ${folder_header})

# Inclue any directories needed for YARP
include_directories(${YARP_INCLUDE_DIRS})
include_directories(${RD_INCLUDE_DIRS})

# Define symbols
if (WIN32)
  ADD_DEFINITIONS(-DSDL_MAIN_HANDLED)
endif (WIN32)

# Set up our main executable.
if (folder_source)
  add_executable(${KEYWORD} ${folder_source} ${folder_header})
  target_link_libraries(${KEYWORD} ${YARP_LIBRARIES} ${RD_LIBRARIES})
  install(TARGETS ${KEYWORD} DESTINATION bin)
else (folder_source)
  message(FATAL_ERROR "No source code files found. Please add something")
endif (folder_source)
ENDIF (ENABLE_rdServer)

