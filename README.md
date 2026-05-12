# TopDownRPG

A top-down RPG combat demo built with **C++** and **raylib**, featuring enemy AI, grid-based pathfinding, and unit-tested core systems.

## Technical Highlights

| System | Details |
|--------|---------|
| Finite State Machine | Enemy AI cycles through Idle → Chase → Attack → Dead states |
| A\* Pathfinding | Enemies navigate around obstacles to reach the player in real time |
| Google Test | 14 unit tests covering path correctness, structure, and edge cases |

## Controls

| Key | Action |
|-----|--------|
| `WASD` | Move |
| `SPACE` | Attack |
| `R` | Restart |

## How to Build

**Requirements:** CMake, raylib, Google Test

```bash
cmake -B build
cmake --build build
./build/game.exe
```

## Run Tests

```bash
cd build
ctest --output-on-failure
```

## Tech Stack

- C++17
- [raylib](https://www.raylib.com/)
- [Google Test](https://github.com/google/googletest)
- CMake