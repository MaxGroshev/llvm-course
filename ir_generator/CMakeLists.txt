cmake_minimum_required(VERSION 3.21)

project(ir_generatore)

#-----------------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
find_package(LLVM REQUIRED CONFIG)

set(SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

if(APPLE)
# set(LLVM_ENABLE_ABI_BREAKING_CHECKS OFF)
# target_compile_definitions(${PROJECT_NAME} PRIVATE LLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING=1)
Set(LINK_OPTION -undefined dynamic_lookup)
endif()
add_executable(${PROJECT_NAME} ${SOURCE})

target_link_options(${PROJECT_NAME} PUBLIC ${LINK_OPTION})
add_definitions(${LLVM_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LLVM_LIBRARIES})
