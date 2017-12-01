# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

set(appname robotDevastation)


file(GLOB conf ${CMAKE_CURRENT_SOURCE_DIR}/conf/*.ini)
#file(GLOB scripts ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*.template)

yarp_install(FILES ${conf} DESTINATION ${RD_CONTEXTS_INSTALL_DIR}/${appname})
#yarp_install(FILES ${scripts} DESTINATION ${RD_APPLICATIONS_TEMPLATES_INSTALL_DIR})
