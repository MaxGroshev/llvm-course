cmake_minimum_required(VERSION 3.21)

project(ir_generator)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

find_package(SDL2 REQUIRED)
message("\n\n God know I hate the man, who united mac+cmake+llvm \n\n")
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

target_link_options(${PROJECT_NAME} PUBLIC ${LINK_OPTION})
add_definitions(${LLVM_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LLVM_LIBRARIES})


target_compile_options(${PROJECT_NAME} PRIVATE ${CPPFLAGS})
target_link_options(${PROJECT_NAME} PRIVATE ${LIBS} ${LDFLAGS})
target_include_directories(${PROJECT_NAME} PUBLIC ${LLVM_INCLUDE_DIR})


find_program(SDL_CONFIG sdl2-config)
add_custom_target(
    run_app_with_generated_ll
    COMMAND clang -S -emit-llvm ${LLVM_OPT_LEVEL}
        -I${CMAKE_CURRENT_SOURCE_DIR}/../graph_app/graphic/
        ${CMAKE_CURRENT_SOURCE_DIR}/../graph_app/app/main.c
        -o ${CMAKE_CURRENT_BUILD_DIR}main.ll 
        &&
        ./${CMAKE_CURRENT_BUILD_DIR}ir_generator "${CMAKE_CURRENT_BUILD_DIR}my_ir.ll"
        &&
        llvm-link ./${CMAKE_CURRENT_BUILD_DIR}main.ll ./${CMAKE_CURRENT_BUILD_DIR}my_ir.ll -o ./${CMAKE_CURRENT_BUILD_DIR}full.bc
        &&
        clang ./${CMAKE_CURRENT_BUILD_DIR}full.bc -L.${CMAKE_CURRENT_BUILD_DIR}/../graph_app/graphic/ -lgraphic $$$(${SDL_CONFIG} --libs) 
        -o ./${CMAKE_CURRENT_BUILD_DIR}custom_app
)

if(DEFINED SDL2_DYN_LIB_PATH)
    add_custom_target(
        run_app_with_generated_ll_interp
        COMMAND clang -S -emit-llvm ${LLVM_OPT_LEVEL}
            -I${CMAKE_CURRENT_SOURCE_DIR}/../graph_app/graphic/
            -I${SDL2_INCLUDE_DIRS}
            ${CMAKE_CURRENT_SOURCE_DIR}/../graph_app/app/main.c
            ${CMAKE_CURRENT_SOURCE_DIR}/../graph_app/graphic/public_interface.c
            &&
            ./${CMAKE_CURRENT_BUILD_DIR}ir_generator "${CMAKE_CURRENT_BUILD_DIR}my_ir.ll"
            &&
            llvm-link ./${CMAKE_CURRENT_BUILD_DIR}main.ll  ./${CMAKE_CURRENT_BUILD_DIR}public_interface.ll ./${CMAKE_CURRENT_BUILD_DIR}my_ir.ll -o ./${CMAKE_CURRENT_BUILD_DIR}full.bc
            &&
            lli --load=${SDL2_DYN_LIB_PATH} ./${CMAKE_CURRENT_BUILD_DIR}full.bc
    )
endif()