# TopDownRPG

## What is this
A top-down RPG combat demo built with C++ and raylib.

## Technical Highlights
- Finite State Machine: enemy AI with Idle / Chase / Attack / Dead states
- A* Pathfinding: enemies navigate around obstacles to reach the player
- Google Test: 14 unit tests covering correctness, path structure, and edge cases

## Controls
- WASD: move
- SPACE: attack
- R: restart

## How to Build
Requires: CMake, raylib, Google Test

cmake -B build && cmake --build build
./build/game.exe
