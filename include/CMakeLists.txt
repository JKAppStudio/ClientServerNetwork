# csNet/include cmake

#csNet include dirs list
list(APPEND CSNET_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Make changes visible to Parent CMakeLists
set(CSNET_INCLUDE_DIRS ${CSNET_INCLUDE_DIRS} PARENT_SCOPE)