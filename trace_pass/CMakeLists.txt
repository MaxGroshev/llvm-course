project(trace_pass)

#-----------------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)

if(APPLE)
    Set(LINK_OPTION -undefined dynamic_lookup)
endif()

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/trace_pass.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${LLVM_INCLUDE_DIRS})
target_link_options(${PROJECT_NAME} PUBLIC ${LINK_OPTION})

target_link_libraries(${PROJECT_NAME} PUBLIC ${LLVM_LIBRARIES})