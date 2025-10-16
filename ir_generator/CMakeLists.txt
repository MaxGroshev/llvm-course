cmake_minimum_required(VERSION 3.21)

project(ir_generatore)

#-----------------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
find_package(LLVM REQUIRED CONFIG)

set(SOURCE
    src/main.cpp
)
add_executable(${PROJECT_NAME} ${SOURCE})

if(APPLE)
    Set(LINK_OPTION -undefined dynamic_lookup)
endif()
target_link_options(${PROJECT_NAME} PRIVATE ${LINK_OPTION})
target_include_directories(${PROJECT_NAME} PRIVATE ${LLVM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LLVM_LIBRARIES})
