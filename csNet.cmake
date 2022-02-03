# csNet cmake module
# Athor: Evgeny Krivoshein
# Setup environment for successful csNet compilation

########################################################################
# Find required components
########################################################################
if(NOT (Boost_FOUND))
    find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
endif()

########################################################################
# Declare csNet cmake variables
########################################################################
set(CSNET_INCLUDE_DIRS)
set(CSNET_SRCS)

########################################################################
# Add subdirectories
########################################################################
add_subdirectory(include)
add_subdirectory(lib)

########################################################################
# Add include paths
########################################################################
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CSNET_INCLUDE_DIRS})
