cmake_minimum_required(VERSION 3.21)
project(asm2ir)

# set(CMAKE_CXX_STANDARD 17)
set(SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/asm2ir.cpp
)

find_package(SDL2 REQUIRED)
find_program(LLVM_CONFIG llvm-config)
if(LLVM_CONFIG)
    execute_process(COMMAND ${LLVM_CONFIG} --includedir OUTPUT_VARIABLE LLVM_INCLUDE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${LLVM_CONFIG} --cppflags OUTPUT_VARIABLE CPPFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${LLVM_CONFIG} --ldflags OUTPUT_VARIABLE LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)  
    execute_process(COMMAND ${LLVM_CONFIG} --libs OUTPUT_VARIABLE LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPPFLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS}")
endif()

find_package(LLVM REQUIRED CONFIG)
add_executable(${PROJECT_NAME} ${SOURCE})

# Bulding graph Library
add_library(MyLibrary STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../graph_app/graphic/public_interface.c
)

target_include_directories(MyLibrary PUBLIC ${SDL2_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../graph_app/)
target_link_libraries(MyLibrary PUBLIC SDL2::SDL2)


target_link_options(${PROJECT_NAME} PUBLIC ${LINK_OPTION})
add_definitions(${LLVM_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PUBLIC ${LLVM_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LLVM_LIBRARIES} SDL2::SDL2 MyLibrary)


target_compile_options(${PROJECT_NAME} PRIVATE ${CPPFLAGS})
target_link_options(${PROJECT_NAME} PRIVATE ${LIBS} ${LDFLAGS})
target_include_directories(${PROJECT_NAME} PUBLIC ${LLVM_INCLUDE_DIR})