cmake_minimum_required(VERSION 3.21)

#-----------------------------------------------------------------------------------------

project(app)

#-----------------------------------------------------------------------------------------

set(CMAKE_CPP_COMPILER clang)
# set(CMAKE_C_COMPILER clang)

#-----------------------------------------------------------------------------------------

set(CMAKE_BUILD_TYPE "Debug")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS  -fsanitize=address)
    add_compile_definitions(MY_DEBUG)
    message("Debug_version")
endif ()

#-----------------------------------------------------------------------------------------

# compile lib for work with grpahic
add_subdirectory(graphic/)
add_subdirectory(app/)

unset(TESTING_ENABLED CACHE)
unset(RUN_SET CACHE)