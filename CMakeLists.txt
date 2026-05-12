cmake_minimum_required(VERSION 3.14)
project(RPGDemo CXX)

set(CMAKE_CXX_STANDARD 17)

# ── Main game ───────────────────────────────────────────────────────────────
set(RAYLIB_INCLUDE "C:/raylib/raylib/src" CACHE PATH "raylib include dir")
set(RAYLIB_LIB     "C:/raylib/raylib/src" CACHE PATH "raylib lib dir")

add_executable(game
    src/main.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/AStar.cpp
)
target_include_directories(game PRIVATE ${RAYLIB_INCLUDE})
target_link_directories(game PRIVATE ${RAYLIB_LIB})
target_link_libraries(game raylib opengl32 gdi32 winmm)

# ── Google Test ─────────────────────────────────────────────────────────────
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ── A* unit tests (no raylib dependency) ────────────────────────────────────
add_executable(test_astar
    src/AStar.cpp
    tests/test_astar.cpp
)
target_include_directories(test_astar PRIVATE src)
target_link_libraries(test_astar GTest::gtest_main)

# ── Demo test ────────────────────────────────────────────────────────────────
add_executable(test_demo
    tests/test_demo.cpp
)
target_link_libraries(test_demo GTest::gtest_main)

enable_testing()
add_test(NAME AStarTests COMMAND test_astar)
add_test(NAME DemoTests  COMMAND test_demo)
