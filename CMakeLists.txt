cmake_minimum_required(VERSION 3.13)

project(null_existence)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-DARCHEUS_INIT -DARCHEUS_DEINIT -g") #TODO: add -Wpedantic

set(ARCHEUS_STD_SDL ON CACHE BOOL "use sdl")

add_subdirectory(lib/arc_cpp_overlay)

add_executable(null_existence
    src/config.cpp
    src/keybinds.cpp
    src/main.cpp

    src/map/map.cpp

    src/menu/start.cpp

    src/simulation/simulation.cpp
    src/simulation/mob/player.cpp

    src/tools/camera/camera.cpp
    src/tools/time/timer.cpp
    src/tools/ui/button.cpp
    src/tools/ui/parallaxlayer.cpp
)

target_include_directories(null_existence
    PRIVATE src
)

if(WIN32 AND NOT MSVC)
    target_link_libraries(null_existence PUBLIC archeus_cpp_overlay mingw32)
else()
    target_link_libraries(null_existence PUBLIC archeus_cpp_overlay)
endif()