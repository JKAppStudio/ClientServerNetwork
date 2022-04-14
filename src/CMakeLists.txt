###########################################
# Athor: Evgeny Krivoshein
# Client Server Network lib
#
# Builds csNet libraty
# #########################################

cmake_minimum_required(VERSION 3.13)

project(csNet_lib 
        VERSION 1.0.0.0 
        LANGUAGES CXX)

############################################
# Find Boost
############################################
if (NOT Boost_FOUND)
    find_package(Boost 1.50.0 REQUIRED)
endif()

############################################
# Specify lib and include directories
############################################
set(csNet_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set(csNet_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include )

############################################
# Specify sources
############################################
set(csNet_souces 
    ${csNet_lib_dir}/client.cpp
    ${csNet_lib_dir}/connection_handler.cpp
    ${csNet_lib_dir}/server.cpp
    )

############################################
# Specify headers
############################################
set(csNet_headers
    ${csNet_include_dir}/client.hpp
    ${csNet_include_dir}/connection_handler.hpp
    ${csNet_include_dir}/server.hpp
    ${csNet_include_dir}/message_handler.hpp
    )

############################################
# Add target library
############################################
add_library(csNet 
            EXCLUDE_FROM_ALL
            ${csNet_souces} ${csNet_headers}
            )

target_include_directories(csNet
        PUBLIC ${Boost_INCLUDE_DIRS} ${csNet_include_dir})


            