To compile your C version of the 2048 game, use the GNU C compiler (gcc).

#### Compile Command
gcc 2048.c -o game
2048.c → your source file
-o game → output executable name

This creates an executable named:

game
Run the Program

On Linux/macOS:

./game





### Compile and Run
Compile
g++ 2048.cpp -o game
Run
./game
Improvements in C++ Version

This version uses:

vector
pair
bool
swap
C++ style I/O (cout)
cleaner row handling

instead of:

raw arrays everywhere
manual copying
C-style patterns




Main C vs C++ Differences

| C               | C++               |
| --------------- | ----------------- |
| Arrays          | `vector`          |
| `printf`        | `cout`            |
| Manual copies   | vector assignment |
| No STL          | STL containers    |
| More procedural | More structured   |





