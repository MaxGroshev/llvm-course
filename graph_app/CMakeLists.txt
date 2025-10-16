project(app)

#-----------------------------------------------------------------------------------------

set(CMAKE_CPP_COMPILER clang++)
# set(CMAKE_C_COMPILER clang)

#-----------------------------------------------------------------------------------------

set(CMAKE_BUILD_TYPE "Debug")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug_version")
endif ()

#-----------------------------------------------------------------------------------------

# compile lib for work with grpahic
# add_dependencies(${PROJECT_NAME} PRIVATE trace_pass)
add_subdirectory(graphic/)
add_subdirectory(app/)

unset(TESTING_ENABLED CACHE)
unset(RUN_SET CACHE)