cmake_minimum_required(VERSION 3.13)

project(maze_challenge)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-DARCHEUS_INIT -DARCHEUS_DEINIT -g") #TODO: add -Wpedantic

set(ARCHEUS_STD_SDL ON CACHE BOOL "use sdl")

add_subdirectory(lib/arc_cpp_overlay)

add_executable(maze_challenge
    src/config.cpp
    src/keybinds.cpp
    src/main.cpp

    src/map/charmap.cpp
    src/map/map.cpp
    src/map/section.cpp
    src/map/tile.cpp

    src/menu/start.cpp

    src/simulation/simulation.cpp

    src/tools/camera/camera.cpp
    src/tools/time/timer.cpp
    src/tools/ui/button.cpp
    src/tools/ui/parallaxlayer.cpp
)

target_include_directories(maze_challenge
    PRIVATE src
)

if(WIN32 AND NOT MSVC)
    target_link_libraries(maze_challenge PUBLIC archeus_cpp_overlay mingw32)
else()
    target_link_libraries(maze_challenge PUBLIC archeus_cpp_overlay)
endif()