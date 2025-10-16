add_executable(${PROJECT_NAME}
    main.c
    abilities/main_ability.c
    ../../trace_pass/src/log.c
)

target_compile_options(${PROJECT_NAME} PUBLIC
    -fpass-plugin=${PASS_OUTPUT}
)

add_custom_target(
    get_app_ir
    COMMAND clang -S -emit-llvm -O3 
        -I${CMAKE_CURRENT_SOURCE_DIR}/../graphic/
        -I${CMAKE_CURRENT_SOURCE_DIR}/../graphic/
        -I${SDL2_INCLUDE_DIRS}/
        ${CMAKE_CURRENT_SOURCE_DIR}/abilities/main_ability.c
        -o ${CMAKE_CURRENT_SOURCE_DIR}/../llvm_ir/main_ability-O3.ll
)

target_link_libraries(${PROJECT_NAME} 
    graphic
)
target_include_directories(${PROJECT_NAME}  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../graphic/
)
