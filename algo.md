This is a complete terminal-based implementation of the game **2048** written in C.
The program:

* Creates a 4×4 board
* Lets the player move tiles using arrow keys or WASD
* Merges equal tiles
* Tracks score
* Ends when no moves remain

---

# Overall Structure

The program is divided into these parts:

1. **Global variables**
2. **Terminal handling**
3. **Board initialization**
4. **Rendering the board**
5. **Tile movement logic**
6. **Input handling**
7. **Game loop**

---

# 1. Header Files

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
```

### Purpose

| Header      | Use                       |
| ----------- | ------------------------- |
| `stdio.h`   | `printf`, `getchar`       |
| `stdlib.h`  | `rand`, `srand`, `atexit` |
| `time.h`    | Random seed               |
| `unistd.h`  | Terminal file descriptors |
| `termios.h` | Raw keyboard input        |

---

# 2. Constants and Global Variables

```c
#define SIZE 4
```

Board size = 4×4.

---

```c
int board[SIZE][SIZE];
int score = 0;
```

* `board` stores tile values
* `score` tracks player score

Example board:

```text
2   0   4   0
0   2   0   8
```

---

# 3. Terminal Raw Mode

Normally terminals wait until Enter is pressed.

Raw mode allows:

* instant key detection
* arrow keys
* no echo

---

## Save original terminal settings

```c
struct termios oldt;
```

Stores old terminal configuration.

---

## disableRawMode()

```c
void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
```

Restores terminal settings when program exits.

---

## enableRawMode()

```c
void enableRawMode() {
```

Enables raw keyboard input.

---

### Get current settings

```c
tcgetattr(STDIN_FILENO, &oldt);
```

---

### Restore terminal on exit

```c
atexit(disableRawMode);
```

---

### Modify settings

```c
newt.c_lflag &= ~(ICANON | ECHO);
```

Disables:

* `ICANON` → line buffering
* `ECHO` → character echoing

So keypresses are immediate and hidden.

---

# 4. clearScreen()

```c
printf("\033[2J\033[H");
```

ANSI escape codes:

* `\033[2J` → clear screen
* `\033[H` → move cursor to top-left

---

# 5. initBoard()

Initializes the game.

---

## Random seed

```c
srand(time(NULL));
```

Ensures different random boards every run.

---

## Fill board with 0

```c
board[i][j] = 0;
```

0 means empty cell.

---

## Add two starting tiles

```c
board[r][c] = 2;
```

Places two random `2`s.

---

# 6. addRandomTile()

Adds:

* 90% chance → `2`
* 10% chance → `4`

---

## Collect empty cells

```c
int empty[SIZE * SIZE][2];
```

Stores coordinates of empty spaces.

---

## Choose random empty position

```c
int idx = rand() % count;
```

---

## Generate tile

```c
board[r][c] = (rand() % 10 == 0) ? 4 : 2;
```

Ternary operator:

* 1/10 probability → 4
* otherwise → 2

---

# 7. printBoard()

Displays the game.

Example:

```text
==== 2048 ====
Score: 24

|   2|   4|    |   8|
|    |   2|    |   4|
```

---

## Empty cells

```c
printf("|    ");
```

---

## Non-empty cells

```c
printf("|%4d", board[i][j]);
```

Right-aligns numbers.

---

# 8. compress()

Moves all nonzero values left.

---

Example:

Before:

```text
[2, 0, 2, 4]
```

After compression:

```text
[2, 2, 4, 0]
```

---

## Logic

```c
if (row[i] != 0)
```

Copy only nonzero numbers.

---

# 9. merge()

Combines adjacent equal numbers.

---

Example:

Before:

```text
[2, 2, 4, 0]
```

After merge:

```text
[4, 0, 4, 0]
```

---

## Score update

```c
score += row[i];
```

Adds merged value to score.

---

# 10. reverse()

Reverses row.

Example:

```text
[2,4,8,16]
```

becomes

```text
[16,8,4,2]
```

Used for right/down movement.

---

# 11. moveLeft()

Core movement logic.

---

For each row:

1. Save old row
2. Compress
3. Merge
4. Compress again
5. Detect changes

---

## Why compress twice?

Example:

```text
[2,0,2,2]
```

After first compress:

```text
[2,2,2,0]
```

After merge:

```text
[4,0,2,0]
```

Second compress:

```text
[4,2,0,0]
```

---

## Detect movement

```c
if (old[j] != board[i][j])
```

Checks whether board changed.

If no change:

* no random tile added

---

# 12. moveRight()

```c
reverse(board[i]);
```

Strategy:

1. Reverse row
2. Use left move logic
3. Reverse back

Efficient reuse of code.

---

# 13. moveUp()

Works column-by-column.

---

## Extract column

```c
temp[row] = board[row][col];
```

---

## Apply left-style logic

```c
compress(temp);
merge(temp);
compress(temp);
```

---

## Write back

```c
board[row][col] = temp[row];
```

---

# 14. moveDown()

Same idea as `moveRight()`:

1. Reverse column
2. Move up logic
3. Reverse back

---

# 15. canMove()

Checks whether game can continue.

---

A move is possible if:

* Any empty cell exists
* Adjacent equal numbers exist

---

## Empty check

```c
if (board[i][j] == 0)
```

---

## Horizontal merge possible

```c
board[i][j] == board[i][j + 1]
```

---

## Vertical merge possible

```c
board[i][j] == board[i + 1][j]
```

---

# 16. getInput()

Reads keyboard input.

---

## Arrow keys in terminal

Arrow keys generate sequences:

```text
ESC [ A
```

where:

* `A` = Up
* `B` = Down
* `C` = Right
* `D` = Left

---

## Mapping

```c
case 'A': return 'w';
```

Converts arrows into WASD internally.

---

# 17. main()

Main game loop.

---

## Enable raw mode

```c
enableRawMode();
```

---

## Initialize board

```c
initBoard();
```

---

# Infinite game loop

```c
while (1)
```

---

## Draw board

```c
printBoard();
```

---

## Game over check

```c
if (!canMove())
```

---

## Read input

```c
char input = getInput();
```

---

## Movement selection

```c
else if (input == 'a')
```

Calls corresponding move function.

---

## Add new tile if movement happened

```c
if (moved) addRandomTile();
```

Very important:

* invalid moves do NOT generate new tiles

---

# Example Flow

Suppose row:

```text
[2,0,2,4]
```

Player presses LEFT.

### Step 1 — Compress

```text
[2,2,4,0]
```

### Step 2 — Merge

```text
[4,0,4,0]
```

### Step 3 — Compress again

```text
[4,4,0,0]
```

Then a random tile appears elsewhere.

---

# Time Complexity

Board size fixed at 4×4.

Operations are effectively constant time:

* Move operations → O(16)
* Rendering → O(16)

Very efficient.

---

# Good Design Choices

This implementation is clean because it:

* Reuses logic (`moveRight` uses `moveLeft`)
* Separates concerns into functions
* Uses raw terminal input
* Properly restores terminal state
* Prevents invalid moves from spawning tiles

---

# Possible Improvements

You could enhance it with:

* Colored tiles
* Win condition (2048 reached)
* Undo feature
* Persistent high scores
* Animations
* Variable board size
* Better UI borders
* AI autoplay

---

# Core Algorithm Summary

Every move follows:

```text
Compress → Merge → Compress
```

That is the heart of 2048.
