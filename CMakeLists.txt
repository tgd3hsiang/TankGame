cmake_minimum_required(VERSION 3.20)
project(TankBattleAI LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Pin a known version so reviewers get reproducible builds.
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
if(EMSCRIPTEN)
    set(PLATFORM Web CACHE STRING "raylib build target" FORCE)
endif()
FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

add_executable(TankBattleAI
    src/main.cpp
    src/Game.cpp
    src/Tank.cpp
    src/TileMap.cpp
    src/Projectile.cpp
    src/EnemyAI.cpp
    src/Pathfinding.cpp
)
target_include_directories(TankBattleAI PRIVATE include)
target_link_libraries(TankBattleAI PRIVATE raylib)

if(EMSCRIPTEN)
    set_target_properties(TankBattleAI PROPERTIES SUFFIX ".html")
    target_link_options(TankBattleAI PRIVATE
        "-sUSE_GLFW=3"
        "-sALLOW_MEMORY_GROWTH=1"
    )
endif()

if(MSVC)
    target_compile_options(TankBattleAI PRIVATE /utf-8)
endif()

if(EMSCRIPTEN)
    set_target_properties(TankBattleAI PROPERTIES
        SUFFIX ".html"
    )

    target_link_options(TankBattleAI PRIVATE
        "--shell-file=${CMAKE_SOURCE_DIR}/web/shell.html"
    )
endif()