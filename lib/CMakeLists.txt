#csNet lib cmake

# csNet sources list
list(APPEND CSNET_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/connection_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/server.cpp
)

# Make changes visible to Parent CMakeLists
set(CSNET_SRCS ${CSNET_SRCS} PARENT_SCOPE)

