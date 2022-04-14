#############################
# athor: Evgeny Krivoshein
#
# Client Server Network
# BOOST:ASIO based project
# Project setup

cmake_minimum_required(VERSION 3.13)
project(csNet CXX)

############################################
# Set C++ standatd
############################################
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

########################################################################
# csNet lib directory
########################################################################
add_subdirectory(src)

########################################################################
# Add examples
########################################################################
add_subdirectory(examples)
