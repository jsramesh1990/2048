
# 2048 - Terminal Edition

A sleek implementation of the addictive 2048 puzzle game in C that runs directly in the Ubuntu 24 terminal. Combine matching numbers, reach 2048, and beyond!

## Features

- 🎮 **Real-time keyboard input** (arrow keys + WASD support)
- 🧩 **Core 2048 mechanics** - merge equal numbers, spawn new tiles
- 📊 **Score tracking** - accumulates merge values
- 🔄 **Matrix shifting logic** - compress, merge, and update rows/columns
- 💀 **Game over detection** - checks for available moves
- 🖥️ **Clean terminal UI** with ANSI escape codes
- ⚡ **Raw terminal mode** using termios (no Enter key required)

## Prerequisites

- **Ubuntu 24** (or any Linux distribution)
- **GCC compiler** (part of build-essential package)
- Standard C libraries (stdio, stdlib, time, unistd, termios)

## Installation

### 1. Install GCC (if not already installed)

```bash
sudo apt update
sudo apt install g++
```

### 2. Create the source file

```bash
nano 2048.c
```

Copy the complete C code from this repository into the file.

### 3. Compile the game

```bash
gcc 2048.c -o 2048
```

For additional warnings (recommended):
```bash
gcc -Wall -Wextra 2048.c -o 2048
```

### 4. Run the game

```bash
./2048
```

## How to Play

### Objective
Combine numbered tiles by moving them in four directions. When two tiles with the same number touch, they merge into one with double the value. Reach the **2048 tile** to win (you can continue beyond for high scores!).

### Controls

| Key | Action |
|-----|--------|
| `←` (Left Arrow) or `A` | Move all tiles left |
| `→` (Right Arrow) or `D` | Move all tiles right |
| `↑` (Up Arrow) or `W` | Move all tiles up |
| `↓` (Down Arrow) or `S` | Move all tiles down |
| `Q` | Quit game |

### Game Rules

1. **Starting state** - Two random tiles (value 2 or rarely 4)
2. **Each move** - All tiles slide as far as possible in chosen direction
3. **Merging** - When two equal tiles collide, they merge into one tile (sum)
4. **Scoring** - Points added equal to the value of merged tiles
5. **New tile spawn** - After each valid move, a new tile (2 or 4) appears in a random empty cell
6. **Game over** - No empty cells AND no adjacent equal tiles in any direction

## Game Display Example

```
==== 2048 ====
Score: 24

|    |   2|    |   4|
|   4|    |   2|    |
|    |    |    |    |
|    |    |    |    |

Use Arrow Keys or WASD. Press q to quit.
```

## Code Structure

### Core Functions

| Function | Purpose |
|----------|---------|
| `initBoard()` | Initializes board with two starting tiles |
| `addRandomTile()` | Spawns a new tile (2 or 4) in random empty cell |
| `printBoard()` | Renders current game state |
| `compress()` | Slides non-zero tiles together (removes gaps) |
| `merge()` | Combines adjacent equal tiles |
| `reverse()` | Reverses row/column order (for opposite direction logic) |
| `moveLeft/Right/Up/Down()` | Movement logic for each direction |
| `canMove()` | Checks if any valid moves remain |
| `enableRawMode()` | Disables terminal line buffering for instant input |
| `getInput()` | Reads keyboard input (supports arrows and WASD) |

### Key C Concepts Demonstrated

- **2D Arrays** - `int board[SIZE][SIZE]` for game state
- **Matrix manipulation** - Row/column shifting and merging
- **Bit manipulation** - `termios` for raw terminal I/O
- **ANSI escape codes** - `\033[2J\033[H]` for screen clearing
- **Random number generation** - `rand()` for tile placement and values
- **Game state management** - Move validation and game over detection

## Algorithm Deep Dive

### Movement Logic (Left as Example)

The movement for each direction follows this pattern:

1. **Compress** - Remove gaps between tiles
   ```
   [2, 0, 2, 0] → [2, 2, 0, 0]
   ```

2. **Merge** - Combine adjacent equal tiles
   ```
   [2, 2, 0, 0] → [4, 0, 0, 0] (score += 4)
   ```

3. **Compress again** - Close any new gaps
   ```
   [4, 0, 0, 0] → [4, 0, 0, 0]
   ```

### Merging Rules

- Each tile can merge **only once per move**
- Merges happen from the direction of movement inward
- Example move left: `[2, 2, 2]` → `[4, 2, 0]` (first two merge, third remains)

### Game Over Detection

The game ends when:
```c
int canMove() {
    // Check for empty cell
    if (board[i][j] == 0) return 1;
    
    // Check for horizontal merge possibility
    if (board[i][j] == board[i][j+1]) return 1;
    
    // Check for vertical merge possibility
    if (board[i][j] == board[i+1][j]) return 1;
}
```

## Terminal Raw Mode Explained

The game uses `termios` to disable line buffering:

```c
newt.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
```

This allows:
- **Instant keypress detection** (no Enter key required)
- **Arrow key support** (3-byte escape sequences)
- **Cleaner input handling**

The `atexit(disableRawMode)` ensures terminal settings are restored on exit.

## Customization Guide

### Change Grid Size

Modify the `SIZE` constant:
```c
#define SIZE 5  // 5x5 grid (harder to reach 2048)
```

### Add Win Condition

Add to `main()` loop:
```c
for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
        if (board[i][j] >= 2048) {
            printf("\n🎉 YOU WIN! 🎉\n");
            // Optionally continue or break
        }
    }
}
```

### Change New Tile Probabilities

Modify in `addRandomTile()`:
```c
board[r][c] = (rand() % 10 == 0) ? 4 : 2;  // 10% chance of 4, 90% chance of 2
```

### Add Color Support

Enhance `printBoard()` with ANSI colors:
```c
if (board[i][j] == 2048) 
    printf("\033[1;33m|%4d\033[0m", board[i][j]);  // Yellow for 2048
else if (board[i][j] >= 1024)
    printf("\033[1;31m|%4d\033[0m", board[i][j]);  // Red for high values
```

## Sample Game Session

```
==== 2048 ====
Score: 0

|    |   2|    |   2|
|    |    |    |    |
|    |    |    |    |
|    |    |    |    |

Press 'd' (right)

==== 2048 ====
Score: 0

|    |    |    |   4|
|    |    |    |    |
|    |    |    |    |
|    |    |    |   2|

Press 'w' (up)
...
```

## Troubleshooting

| Issue | Solution |
|-------|----------|
| Arrow keys not working | Use WASD keys instead; check terminal compatibility |
| Screen not clearing | Ensure terminal supports ANSI escape codes (most do) |
| Compilation errors with termios | Include `<termios.h>` header (already in code) |
| Game ends prematurely | Check `canMove()` logic; ensure merges are possible |
| Terminal behaves strangely after exit | Run `reset` command; the `atexit()` handler should restore |
| No tiles spawning | Verify `addRandomTile()` finds empty cells |

## Possible Enhancements

After mastering the basic version, try adding:

- 🎨 **ncurses graphics** - Colored tiles, smoother rendering
- 💾 **High score persistence** - Save best scores to file
- 🔄 **Undo move** - Stack-based move history
- 🏆 **Achievement system** - Track milestones (512, 1024, 2048)
- 📊 **Move counter and timer** - Track how long/efficient your game was
- 🎮 **Animation** - Smooth tile sliding effects
- 🌐 **Network multiplayer** - Compete with friends
- 🤖 **AI solver** - Computer plays using expectimax algorithm
- 📈 **Statistics tracking** - Win rate, average score, best tile

## Advanced Enhancement: Score File

Add high score persistence:

```c
void saveHighScore() {
    FILE *f = fopen("2048.scores", "w");
    if (f) fprintf(f, "%d", score);
    fclose(f);
}

int loadHighScore() {
    FILE *f = fopen("2048.scores", "r");
    int hs = 0;
    if (f) fscanf(f, "%d", &hs);
    fclose(f);
    return hs;
}

// In main() after game over
int highScore = loadHighScore();
if (score > highScore) {
    printf("New high score! 🏆\n");
    saveHighScore();
}
```

## Building with Makefile

Create a `Makefile`:

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = 2048
SOURCES = 2048.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

debug: $(SOURCES)
	$(CC) -g -o $(TARGET)-debug $(SOURCES)
	gdb $(TARGET)-debug
```

Usage:
```bash
make        # Compile
make run    # Compile and run
make clean  # Remove executable
```

## Learning Path Progression

After 2048, challenge yourself with:

1. **Tic-Tac-Toe** - 2D arrays, win conditions
2. **Hangman** - Strings, file I/O
3. **Snake** - Real-time input, ncurses
4. **Minesweeper** - Recursion, flood fill
5. **2048** (this project) - Matrix manipulation, game AI
6. **Chess Engine** - Complex move generation, minimax
7. **Raycasting Engine** - 2.5D graphics, trigonometry

## Resources

- [2048 Game Original](https://play2048.co/) - The original browser game
- [Termios Manual](https://man7.org/linux/man-pages/man3/termios.3.html) - Terminal I/O control
- [ANSI Escape Codes](https://gist.github.com/fnky/458719343aabd01cfb17a7a4f6a1285c) - Terminal colors and formatting
- [C Reference](https://en.cppreference.com/) - Standard library documentation

## License

Open source for educational purposes.

---

## Author

Created as a C programming project demonstrating matrix manipulation and terminal raw mode input on Ubuntu 24.

**Merge wisely and reach 2048! 🎯🔢**
```

