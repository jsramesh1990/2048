# Program Flow of the 2048

Here’s the complete execution flow of your program from start to finish.

---

# High-Level Flow

```text
Start Program
    ↓
Enable Raw Mode
    ↓
Initialize Board
    ↓
Main Game Loop
    ├── Print Board
    ├── Check Game Over
    ├── Read Input
    ├── Perform Move
    ├── Add Random Tile
    └── Repeat
    ↓
Restore Terminal
    ↓
Exit
```

---

# Detailed Flow

## 1. Program Starts

Execution begins from:

```c
int main()
```

---

# 2. Enable Raw Terminal Mode

```c
enableRawMode();
```

Purpose:

* Read keys instantly
* Disable Enter requirement
* Hide typed characters

---

## Inside `enableRawMode()`

### Save current terminal settings

```c
tcgetattr(STDIN_FILENO, &oldt);
```

↓

### Disable canonical mode and echo

```c
newt.c_lflag &= ~(ICANON | ECHO);
```

↓

### Apply new settings

```c
tcsetattr(STDIN_FILENO, TCSANOW, &newt);
```

↓

### Register cleanup function

```c
atexit(disableRawMode);
```

When program exits:

* terminal restores automatically

---

# 3. Initialize Game Board

```c
initBoard();
```

---

## Inside `initBoard()`

### Seed random generator

```c
srand(time(NULL));
```

↓

### Set all cells to 0

```text
0 0 0 0
0 0 0 0
0 0 0 0
0 0 0 0
```

↓

### Place first random tile

Example:

```text
0 0 2 0
```

↓

### Place second random tile

Example:

```text
0 2 2 0
```

Board ready.

---

# 4. Enter Main Game Loop

```c
while (1)
```

This loop runs continuously until:

* user quits
* game over occurs

---

# MAIN LOOP FLOW

---

# Step A — Print Board

```c
printBoard();
```

---

## Inside `printBoard()`

### Clear terminal

```c
clearScreen();
```

↓

### Print title

```text
==== 2048 ====
```

↓

### Print score

```text
Score: 12
```

↓

### Print board row by row

Example:

```text
|   2|   4|    |   8|
|    |   2|    |   4|
```

↓

### Print controls

```text
Use Arrow Keys or WASD
```

---

# Step B — Check if Game Can Continue

```c
if (!canMove())
```

---

## Inside `canMove()`

Program scans all cells.

---

### Condition 1 — Empty cell exists?

```c
if (board[i][j] == 0)
```

YES → game continues.

---

### Condition 2 — Horizontal merge possible?

```c
board[i][j] == board[i][j + 1]
```

YES → game continues.

---

### Condition 3 — Vertical merge possible?

```c
board[i][j] == board[i + 1][j]
```

YES → game continues.

---

If none are true:

```text
Game Over
```

Loop breaks.

---

# Step C — Read User Input

```c
char input = getInput();
```

---

# Inside `getInput()`

## User presses key

Could be:

* WASD
* Arrow keys
* q

---

## If Arrow Key

Arrow keys produce:

```text
ESC [ A
```

Program reads sequence:

```c
if (c == 27)
```

↓

Maps:

| Key   | Returns |
| ----- | ------- |
| Up    | w       |
| Down  | s       |
| Left  | a       |
| Right | d       |

---

# Step D — Decide Move

```c
if (input == 'a')
```

---

Possible calls:

| Input | Function    |
| ----- | ----------- |
| a     | moveLeft()  |
| d     | moveRight() |
| w     | moveUp()    |
| s     | moveDown()  |
| q     | quit        |

---

# Step E — Movement Logic

Suppose player presses LEFT.

Program calls:

```c
moveLeft();
```

---

# Inside `moveLeft()`

For EACH row:

---

## 1. Save old row

```c
old[j] = board[i][j];
```

Used later to detect changes.

---

## 2. Compress

Moves nonzero values left.

Example:

```text
[2,0,2,4]
↓
[2,2,4,0]
```

---

## 3. Merge

Equal adjacent tiles combine.

```text
[2,2,4,0]
↓
[4,0,4,0]
```

Score updated.

---

## 4. Compress Again

```text
[4,0,4,0]
↓
[4,4,0,0]
```

---

## 5. Detect Changes

Compare old row vs new row.

If different:

```c
changed = 1;
```

---

# Step F — Add Random Tile

Back in `main()`:

```c
if (moved)
    addRandomTile();
```

---

# Inside `addRandomTile()`

## Find all empty cells

Example:

```text
(0,1)
(2,3)
(3,0)
```

---

## Pick random empty position

```c
rand() % count
```

---

## Add:

* 90% → 2
* 10% → 4

Board updates.

---

# Step G — Repeat Loop

Program returns to:

```c
while (1)
```

And repeats:

```text
Print
Check
Input
Move
Spawn Tile
Repeat
```

---

# Example Full Turn

---

## Initial Board

```text
2 0 2 4
0 4 0 4
2 2 0 0
0 0 0 0
```

---

## User presses LEFT

↓

### Row 1

```text
2 0 2 4
↓ compress
2 2 4 0
↓ merge
4 0 4 0
↓ compress
4 4 0 0
```

---

### Row 2

```text
0 4 0 4
↓
4 4 0 0
↓
8 0 0 0
```

---

### Row 3

```text
2 2 0 0
↓
4 0 0 0
```

---

## New Board

```text
4 4 0 0
8 0 0 0
4 0 0 0
0 0 0 0
```

---

## Add Random Tile

Example:

```text
4 4 0 0
8 0 2 0
4 0 0 0
0 0 0 0
```

---

# Flow of Right/Up/Down Moves

## RIGHT

```text
Reverse Row
↓
Move Left Logic
↓
Reverse Back
```

---

## UP

```text
Extract Column
↓
Compress
↓
Merge
↓
Compress
↓
Write Back
```

---

## DOWN

```text
Reverse Column
↓
Move Up Logic
↓
Reverse Back
```

---

# Program End Flow

Program exits when:

## User presses:

```text
q
```

OR

## No moves remain

```text
Game Over!
```

---

# Exit Cleanup

Because of:

```c
atexit(disableRawMode);
```

Terminal settings restore automatically.

Without this:

* terminal could become unusable temporarily

---

# Core Gameplay Cycle

```text
INPUT
  ↓
MOVE
  ↓
COMPRESS
  ↓
MERGE
  ↓
COMPRESS
  ↓
ADD RANDOM TILE
  ↓
RENDER BOARD
  ↓
REPEAT
```

This loop is the heart of the entire game.
