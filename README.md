# Terminal Pacman Game

A classic Pacman game implementation that runs in the terminal, built in C as an APSC 142 engineering programming project.

## Game Overview

Navigate Pacman through a maze, collect all the dots, and avoid the two ghosts hunting you down! The game features intelligent ghost AI that can spot you when you're in their line of sight.

## Features

- **Terminal-based gameplay** - Runs entirely in your command line
- **WASD controls** - Simple and intuitive keyboard navigation
- **2 Ghost enemies** - Ghosts move randomly but will chase you if they see you
- **Dynamic game map** - Customizable maze layout via `map.txt`
- **Win/Loss conditions** - Collect all dots to win, don't get caught by ghosts!
- **Color-coded display** - Enhanced visual experience with colored characters

## Controls

| Key | Action |
|-----|--------|
| `W` | Move Up |
| `A` | Move Left |
| `S` | Move Down |
| `D` | Move Right |

## Installation

### Prerequisites

- GCC compiler (C99 standard or later)
- CMake (version 3.26 or higher)
- A terminal that supports ANSI color codes

### Build Instructions

1. Clone or download this repository
2. Navigate to the project directory
3. Create a build directory and compile:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Alternatively, if using CLion or another IDE with CMake support, simply open the project and build.

## Running the Game

After building, run the executable from the build directory:

```bash
./project
```

**Important:** Make sure to run the game in a terminal that emulates output properly. If using an IDE like CLion, enable "Emulate terminal in the output console" in your run configuration.

## Project Structure

```
project/
├── apsc142project.c    # Main game loop and entry point
├── colours.c/h         # Terminal color output and input handling
├── defines.h           # Game constants and character definitions
├── game.c/h            # Core game logic (win/loss conditions, movement)
├── ghost.c/h           # Ghost AI and behavior
├── map.c/h             # Map loading and rendering
├── map.txt             # Game map layout file
├── CMakeLists.txt      # Build configuration
├── doctest.h           # Testing framework header
└── tests.cpp           # Unit tests
```

## Game Mechanics

### Map Layout

The map is defined in `map.txt` using the following characters:
- `P` - Pacman starting position
- `G` - Ghost starting positions (exactly 2 required)
- `.` - Collectible dots
- `W` - Walls (impassable)
- ` ` (space) - Empty walkable space

### Win Condition

Collect all the dots on the map without being caught by the ghosts.

### Loss Condition

If Pacman occupies the same position as a ghost, you lose the game.

### Ghost AI

- Ghosts move randomly by default
- When a ghost has line of sight to Pacman (horizontal or vertical), it will chase directly toward you
- Ghosts respect walls and cannot pass through them
- Dots reappear after ghosts move over them

## Customization

You can create your own maps by editing `map.txt`. Follow these rules:

1. Use the characters defined above (`P`, `G`, `.`, `W`, ` `)
2. Include exactly 1 Pacman (`P`)
3. Include exactly 2 Ghosts (`G`)
4. Create interesting maze patterns with walls (`W`)
5. Place dots (`.`) for Pacman to collect

## Development

### Running Tests

The project includes unit tests built with doctest:

```bash
./tests
```

### Code Organization

The project follows modular design principles:
- **Separation of concerns**: Map handling, game logic, and ghost AI are in separate modules
- **Global state management**: Map dimensions and data are stored as global variables for easy access
- **Dynamic memory allocation**: Maps are loaded dynamically based on file contents

## Technical Details

- **Language**: C (C99 standard)
- **Compiler Flags**: Strict error checking with `-Wall`, `-pedantic`, `-Werror`
- **Memory Management**: Dynamic allocation with proper cleanup
- **Input Handling**: Non-blocking character input using `getch()`
- **Display**: ANSI color codes for terminal output

## Error Codes

The program returns specific error codes:
- `0` (NO_ERROR) - Game completed successfully
- `1` (ERR_NO_MAP) - Map file not found
- `2` (ERR_NO_PACMAN) - No Pacman found on map
- `3` (ERR_NO_GHOSTS) - Fewer than 2 ghosts found on map

## Known Limitations

- Requires terminal with ANSI color support
- Input is processed one character at a time (no buffering)
- Map must be rectangular and well-formed
- No save/load game functionality

## Credits

- **Project Framework**: APSC 142 Engineering Programming
- **Copyright**: Sean Kauffman 2024

## License

This is an educational project for APSC 142. Check with your institution regarding usage and distribution policies.

---

Enjoy playing Terminal Pacman! 👻🟡
