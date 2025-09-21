cmake_minimum_required(VERSION 3.21)

#-----------------------------------------------------------------------------------------

add_executable(${PROJECT_NAME}
    main.c
    abilities/main_ability.c
)

target_link_libraries(${PROJECT_NAME} 
    graphic
)
target_include_directories(${PROJECT_NAME}  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../graphic/
)
