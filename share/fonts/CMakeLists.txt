# Copyright: 2014 ASROB
# Author: Juan G Victores
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

set(appname fonts)

#file(GLOB conf ${CMAKE_CURRENT_SOURCE_DIR}/conf/*.ini)
#file(GLOB scripts ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*.template)

file(GLOB fonts ${CMAKE_CURRENT_SOURCE_DIR}/*.ttf)

#yarp_install(FILES ${conf} DESTINATION ${RD_CONTEXTS_INSTALL_DIR}/${appname})
#yarp_install(FILES ${scripts} DESTINATION ${RD_APPLICATIONS_TEMPLATES_INSTALL_DIR})

yarp_install(FILES ${fonts} DESTINATION ${RD_CONTEXTS_INSTALL_DIR}/${appname})

