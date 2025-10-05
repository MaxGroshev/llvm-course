project(graphic)

#-----------------------------------------------------------------------------------------

find_package(SDL2 REQUIRED)

set(SOURCE
    public_interface.c
)

add_library(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)
