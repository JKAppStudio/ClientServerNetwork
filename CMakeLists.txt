#############################
# athor: Evgeny Krivoshein
#
# Server Client Network
# BOOST:ASIO based project

#
# Project setup

cmake_minimum_required(VERSION 3.10)
project(csNet CXX)

########################################################################
# Set C++ standatd
########################################################################
set(CMAKE_CXX_STANDARD 17)

########################################################################
# Find required modules
########################################################################
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

########################################################################
# Local Include Dir
########################################################################
include_directories(${CMAKE_BINARY_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/include/*.hpp
    ${CMAKE_SOURCE_DIR}/lib/*.cpp
)

#add_subdirectory(include)

add_executable(csNetTest csNetTest.cpp ${SOURCES})
