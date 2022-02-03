#############################
# athor: Evgeny Krivoshein
#
# Client Server Network
# BOOST:ASIO based project
# Project setup

cmake_minimum_required(VERSION 3.10)
project(csNet CXX)

########################################################################
# Add required cmake modules paths
########################################################################
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR})
set(CSNET_SOURCE_DIR ${CMAKE_SOURCE_DIR})

########################################################################
# Set C++ standatd
########################################################################
set(CMAKE_CXX_STANDARD 17)

########################################################################
# Include cmake modules
########################################################################
include(csNet)

add_executable(csNet_example csNet_example.cpp ${CSNET_SRCS})
