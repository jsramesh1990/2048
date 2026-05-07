#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

#define SIZE 4

int board[SIZE][SIZE];
int score = 0;

// Terminal raw mode
struct termios oldt;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void enableRawMode() {
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    atexit(disableRawMode);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void clearScreen() {
    printf("\033[2J\033[H");
}

void initBoard() {
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = 0;

    // Start with 2 tiles
    for (int i = 0; i < 2; i++) {
        int r, c;
        do {
            r = rand() % SIZE;
            c = rand() % SIZE;
        } while (board[r][c] != 0);
        board[r][c] = 2;
    }
}

void addRandomTile() {
    int empty[SIZE * SIZE][2];
    int count = 0;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                empty[count][0] = i;
                empty[count][1] = j;
                count++;
            }
        }
    }

    if (count > 0) {
        int idx = rand() % count;
        int r = empty[idx][0];
        int c = empty[idx][1];
        board[r][c] = (rand() % 10 == 0) ? 4 : 2;
    }
}

void printBoard() {
    clearScreen();
    printf("==== 2048 ====\n");
    printf("Score: %d\n\n", score);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0)
                printf("|    ");
            else
                printf("|%4d", board[i][j]);
        }
        printf("|\n");
    }

    printf("\nUse Arrow Keys or WASD. Press q to quit.\n");
}

void compress(int row[]) {
    int temp[SIZE] = {0};
    int pos = 0;

    for (int i = 0; i < SIZE; i++) {
        if (row[i] != 0) {
            temp[pos++] = row[i];
        }
    }

    for (int i = 0; i < SIZE; i++) {
        row[i] = temp[i];
    }
}

void merge(int row[]) {
    for (int i = 0; i < SIZE - 1; i++) {
        if (row[i] != 0 && row[i] == row[i + 1]) {
            row[i] *= 2;
            score += row[i];
            row[i + 1] = 0;
        }
    }
}

void reverse(int row[]) {
    for (int i = 0; i < SIZE / 2; i++) {
        int temp = row[i];
        row[i] = row[SIZE - i - 1];
        row[SIZE - i - 1] = temp;
    }
}

int moveLeft() {
    int changed = 0;

    for (int i = 0; i < SIZE; i++) {
        int old[SIZE];
        for (int j = 0; j < SIZE; j++) old[j] = board[i][j];

        compress(board[i]);
        merge(board[i]);
        compress(board[i]);

        for (int j = 0; j < SIZE; j++) {
            if (old[j] != board[i][j]) changed = 1;
        }
    }
    return changed;
}

int moveRight() {
    for (int i = 0; i < SIZE; i++) reverse(board[i]);
    int changed = moveLeft();
    for (int i = 0; i < SIZE; i++) reverse(board[i]);
    return changed;
}

int moveUp() {
    int changed = 0;

    for (int col = 0; col < SIZE; col++) {
        int temp[SIZE], old[SIZE];

        for (int row = 0; row < SIZE; row++) {
            temp[row] = board[row][col];
            old[row] = temp[row];
        }

        compress(temp);
        merge(temp);
        compress(temp);

        for (int row = 0; row < SIZE; row++) {
            if (board[row][col] != temp[row]) changed = 1;
            board[row][col] = temp[row];
        }
    }
    return changed;
}

int moveDown() {
    int changed = 0;

    for (int col = 0; col < SIZE; col++) {
        int temp[SIZE];

        for (int row = 0; row < SIZE; row++)
            temp[row] = board[row][col];

        reverse(temp);
        compress(temp);
        merge(temp);
        compress(temp);
        reverse(temp);

        for (int row = 0; row < SIZE; row++) {
            if (board[row][col] != temp[row]) changed = 1;
            board[row][col] = temp[row];
        }
    }
    return changed;
}

int canMove() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) return 1;
            if (j < SIZE - 1 && board[i][j] == board[i][j + 1]) return 1;
            if (i < SIZE - 1 && board[i][j] == board[i + 1][j]) return 1;
        }
    }
    return 0;
}

char getInput() {
    char c = getchar();

    if (c == 27) { // Arrow keys
        getchar(); // skip '['
        switch (getchar()) {
            case 'A': return 'w'; // up
            case 'B': return 's'; // down
            case 'C': return 'd'; // right
            case 'D': return 'a'; // left
        }
    }
    return c;
}

int main() {
    enableRawMode();
    initBoard();

    while (1) {
        printBoard();

        if (!canMove()) {
            printf("\nGame Over! Final Score: %d\n", score);
            break;
        }

        char input = getInput();
        int moved = 0;

        if (input == 'q') break;
        else if (input == 'a') moved = moveLeft();
        else if (input == 'd') moved = moveRight();
        else if (input == 'w') moved = moveUp();
        else if (input == 's') moved = moveDown();

        if (moved) addRandomTile();
    }

    return 0;
}
