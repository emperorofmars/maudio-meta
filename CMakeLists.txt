cmake_minimum_required(VERSION 3.0)
project(maudio)

set(PORTABLE 1 CACHE BOOL "set portable install")
set(DEBIAN 0 CACHE BOOL "create Debian package")

set(MAUDIO_PARENT_PROJECT 1)

if(PORTABLE EQUAL 1)
	message(STATUS "Using portable install")
	
	set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/deploy")
	add_definitions("-DMAUDIO_PORTABLE=1")
else()
	message(STATUS "Using system install")
	
	add_definitions("-DMAUDIO_PORTABLE=0")
endif()

set(MAUDIO_LIB_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/libmaudio/include")
set(MAUDIO_LIB_SOURCE_DIR "${PROJECT_SOURCE_DIR}/libmaudio/src")
set(MAUDIO_LIB_TEST_DIR "${PROJECT_SOURCE_DIR}/libmaudio/test")

include_directories(${MAUDIO_LIB_INCLUDE_DIR})
include_directories(${MAUDIO_LIB_SOURCE_DIR})
include_directories(${MAUDIO_LIB_TEST_DIR})

enable_testing()

add_subdirectory("libmaudio")
add_subdirectory("maudio-plugins")
add_subdirectory("maudio-editor-qt")
add_subdirectory("examples")

#create packages
if(PORTABLE EQUAL 1)
	message(STATUS "Using zip package generator")
	set(CPACK_GENERATOR "ZIP")
elseif(${DEBIAN} EQUAL 1)
	message(STATUS "Using Debian package generator")
	set(CPACK_GENERATOR "DEB")
endif()

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Martin Schwarz")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MAudio")
set(CPACK_PACKAGE_VENDOR "Martin Schwarz")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_OUTPUT_FILE_PREFIX "packages")
INCLUDE(CPack)

