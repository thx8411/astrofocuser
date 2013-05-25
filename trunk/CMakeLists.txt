####################################
#   CMakeLists for astrofocuser    #
# (c) Blaise-Florentin Collin 2013 #
#       under GPL v3 License       #
####################################  

cmake_minimum_required(VERSION 2.8.6)


####################
# BUILD DIRECTIVES #
####################

# distribution detection
find_file(DEBIAN_FOUND debian_version debconf.conf PATHS /etc)
find_file(FEDORA_FOUND fedora-release PATHS /etc)
find_file(MAGEIA_FOUND mageia-release PATHS /etc)

# project name
project(astrofocuser)

# version
set(DEB_EPOCH 0)
set(MAJOR_VERSION "1")
set(MINOR_VERSION "1")
set(PATCH_VERSION "3")
set(BUILD_VERSION "5")
set(FULL_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D BINARY_VERSION=\\\"${FULL_VERSION}\\\" -D BUILD_VERSION=${BUILD_VERSION}")

# destir for binary packages
set($ENV{DESTDIR} "${CMAKE_BINARY_DIR}/fakeinstall")

# needed packages
find_package(Qt4 REQUIRED)
if(NOT QT4_FOUND)
   message(FATAL_ERROR "Qt4 not found, generation stopped")
endif(NOT QT4_FOUND)

find_package(X11 REQUIRED)
if(NOT X11_Xcomposite_FOUND)
   message(FATAL_ERROR "X11 composite extensions missing")
else(NOT X11_Xcomposite_FOUND)
   include_directories(${X11_INCLUDE_DIR} ${X11_Xcomposite_INCLUDE_PATH})
endif(NOT X11_Xcomposite_FOUND)

# source files
set(astrofocuser_SOURCES
	main.cpp
	qfwhm.cpp)

# we use Qt4
set(CMAKE_AUTOMOC TRUE)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# target
add_executable(astrofocuser ${astrofocuser_SOURCES})

# target libs
target_link_libraries(astrofocuser ${QT_LIBRARIES} ${X11_LIBRARIES} ${X11_Xcomposite_LIB})

# installation
install(TARGETS astrofocuser RUNTIME DESTINATION /usr/bin)
install(FILES astrofocuser-icon.png DESTINATION /usr/share/icons)
install(FILES astrofocuser-icon.png DESTINATION /usr/share/astrofocuser/icons)
install(FILES ChangeLog.txt DESTINATION /usr/share/astrofocuser)
install(FILES license.txt DESTINATION /usr/share/astrofocuser)
install(FILES README DESTINATION /usr/share/astrofocuser)
install(FILES astrofocuser-entry.desktop DESTINATION /usr/share/applications)


######################
# PACKAGE GENERATION #
######################

# default arch
set(ARCHITECTURE "unkown")

# global directives
set(CPACK_PACKAGE_VERSION_MAJOR ${MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${PATCH_VERSION})


##################
# source package #
##################

# source package
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES ".svn")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "astrofocuser_${FULL_VERSION}_src" CACHE INTERNAL "tarball basename")


#####################
# binaries packages #
#####################

# global directives for debian and rpm packages
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Focus helper for astronomy imaging")
set(CPACK_PACKAGE_DESCRIPTION "astrofocuser is a small tool for astronomy imaging.")
set(CPACK_PACKAGE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION} It helps to focus using the fwhm method.")
set(CPACK_PACKAGE_VENDOR "Thx8411")
set(CPACK_PACKAGE_CONTACT "thx8411@users.sourceforge.net")
set(CPACK_STRIP_FILES astrofocuser)


##################
# debian package #
##################

if(DEBIAN_FOUND)

   # arch detection
   find_program(DPKG_PROGRAM dpkg DOC "dpkg program of Debian-based systems")
   if(DPKG_PROGRAM)
     execute_process(
        COMMAND ${DPKG_PROGRAM} --print-architecture
        OUTPUT_VARIABLE ARCHITECTURE
        OUTPUT_STRIP_TRAILING_WHITESPACE
     )
   endif(DPKG_PROGRAM)

   # settings
   set(CPACK_GENERATOR DEB)
   set(CPACK_SET_DESTDIR "ON")
   set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${ARCHITECTURE})
   set(CPACK_DEBIAN_PACKAGE_NAME "astrofocuser")

   # dependencies
   set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libgcc1, libqtcore4, libqtgui4, libstdc++6, libxcomposite1")
   set(CPACK_DEBIAN_PACKAGE_SECTION graphics)
   set(CPACK_DEBIAN_PACKAGE_VERSION "${DEB_EPOCH}:${FULL_VERSION}-1")
   set(CPACK_PACKAGE_FILE_NAME ${CPACK_DEBIAN_PACKAGE_NAME}_${FULL_VERSION}_${ARCHITECTURE})
   set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}\n ${CPACK_PACKAGE_DESCRIPTION}")

endif(DEBIAN_FOUND)


###############
# rpm package #
###############

if(FEDORA_FOUND OR MAGEIA_FOUND)

   # arch detection
   execute_process(COMMAND  uname -m
      OUTPUT_VARIABLE ARCHITECTURE
      OUTPUT_STRIP_TRAILING_WHITESPACE
   )

   # settings
   set(CPACK_GENERATOR RPM)
   set(CPACK_SET_DESTDIR "OFF")
   set(CPACK_RPM_PACKAGE_ARCHITECTURE ${ARCHITECTURE})
   set(CPACK_RPM_PACKAGE_LICENSE "GPL v3")
   set(CPACK_RPM_PACKAGE_GROUP "Productivity/Scientific/Astronomy")
   set(CPACK_RPM_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION})
   set(CPACK_RPM_PACKAGE_NAME "astrofocuser")
   set(CPACK_PACKAGE_FILE_NAME ${CPACK_RPM_PACKAGE_NAME}_${FULL_VERSION}.${ARCHITECTURE})
   set(CPACK_RPM_PACKAGE_RELOCATABLE FALSE)

   # dependencies
   if(ARCHITECTURE STREQUAL "x86_64")
      set(CPACK_RPM_PACKAGE_REQUIRES "lib64qtcore4,")
      set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES} lib64qtgui4,")
      set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES} lib64xcomposite1")
   else(ARCHITECTURE STREQUAL "x86_64")
      set(CPACK_RPM_PACKAGE_REQUIRES "libqtcore4,")
      set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES} libqtgui4,")
      set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES} libxcomposite1")
   endif(ARCHITECTURE STREQUAL "x86_64")

endif(FEDORA_FOUND OR MAGEIA_FOUND)

include(CPack)
