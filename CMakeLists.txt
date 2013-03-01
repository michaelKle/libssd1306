 # the name of your project
project(libssd1306)

cmake_minimum_required(VERSION 2.4.0)

add_subdirectory(src)


################################################################################
#
# Installation commands
#
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ssd1306")

set(CPACK_SOURCE_GENERATOR "TGZ")

set(CPACK_PACKAGE_VENDOR "Michael Kleiber")

set(CPACK_SOURCE_IGNORE_FILES
"~$"
".svn"
".project"
".cproject"
"*.*"
"${PROJECT_SOURCE_DIR}/build/"
"${PROJECT_SOURCE_DIR}/packaging/debian"
)


set(CPACK_STRIP_FILES TRUE)

INCLUDE(CPack)
