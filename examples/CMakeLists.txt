cmake_minimum_required(VERSION 3.0)
project(libmaudio)

if(NOT DEFINED MAUDIO_PARENT_PROJECT)
	set(MAUDIO_PARENT_PROJECT 0)
endif()

set(PORTABLE 1 CACHE BOOL "set portable install")
set(DEBIAN 0 CACHE BOOL "create Debian package")

if(PORTABLE EQUAL 1)
	message(STATUS "example: using portable install")
	
	set(CMAKE_INSTALL_PREFIX "install")
	add_definitions("-DMAUDIO_PORTABLE=1")
else()
	message(STATUS "example: using system install")
	
	add_definitions("-DMAUDIO_PORTABLE=0")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wl,--no-as-needed")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "./res/lib:.")

set(TESTBIN_SOURCE_FILES
	./test1.cpp
)

add_executable(maudio_example ${TESTBIN_SOURCE_FILES})
target_link_libraries(maudio_example maudio)

#install lib to prefix
if(PORTABLE EQUAL 1)
	install(TARGETS maudio_example RUNTIME DESTINATION ".")
else()
	#install(TARGETS maudio LIBRARY DESTINATION "lib")
	#install headers to prefix
endif()





