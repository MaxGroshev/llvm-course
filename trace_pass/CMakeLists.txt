project(trace_pass)

#-----------------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})

if(APPLE)
    Set(LINK_OPTION -undefined dynamic_lookup)
endif()

add_custom_target(
    build_pass
    COMMAND clang++ ${CMAKE_CURRENT_SOURCE_DIR}/src/trace_pass.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/log.c 
    ${LINK_OPTION} -fPIC -shared -I${LLVM_INCLUDE_DIRS}/ -L${LLVM_LIBRARIES}/ 
    -o ${PASS_OUTPUT}
)