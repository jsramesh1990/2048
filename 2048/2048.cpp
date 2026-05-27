#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>

using namespace std;

const int SIZE = 4;

vector<vector<int>> board(SIZE, vector<int>(SIZE, 0));
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
    cout << "\033[2J\033[H";
}

void initBoard() {
    srand(time(nullptr));

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
    vector<pair<int, int>> emptyCells;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                emptyCells.push_back({i, j});
            }
        }
    }

    if (!emptyCells.empty()) {
        int idx = rand() % emptyCells.size();

        int r = emptyCells[idx].first;
        int c = emptyCells[idx].second;

        board[r][c] = (rand() % 10 == 0) ? 4 : 2;
    }
}

void printBoard() {
    clearScreen();

    cout << "====== 2048 ======\n";
    cout << "Score: " << score << "\n\n";

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {

            if (board[i][j] == 0)
                cout << "|    ";
            else {
                printf("|%4d", board[i][j]);
            }
        }

        cout << "|\n";
    }

    cout << "\nUse Arrow Keys or WASD. Press q to quit.\n";
}

void compress(vector<int>& row) {
    vector<int> temp(SIZE, 0);

    int pos = 0;

    for (int i = 0; i < SIZE; i++) {
        if (row[i] != 0) {
            temp[pos++] = row[i];
        }
    }

    row = temp;
}

void merge(vector<int>& row) {
    for (int i = 0; i < SIZE - 1; i++) {

        if (row[i] != 0 && row[i] == row[i + 1]) {

            row[i] *= 2;
            score += row[i];

            row[i + 1] = 0;
        }
    }
}

void reverseRow(vector<int>& row) {
    for (int i = 0; i < SIZE / 2; i++) {
        swap(row[i], row[SIZE - i - 1]);
    }
}

bool moveLeft() {
    bool changed = false;

    for (int i = 0; i < SIZE; i++) {

        vector<int> old = board[i];

        compress(board[i]);
        merge(board[i]);
        compress(board[i]);

        if (old != board[i]) {
            changed = true;
        }
    }

    return changed;
}

bool moveRight() {

    for (int i = 0; i < SIZE; i++) {
        reverseRow(board[i]);
    }

    bool changed = moveLeft();

    for (int i = 0; i < SIZE; i++) {
        reverseRow(board[i]);
    }

    return changed;
}

bool moveUp() {
    bool changed = false;

    for (int col = 0; col < SIZE; col++) {

        vector<int> temp(SIZE);

        for (int row = 0; row < SIZE; row++) {
            temp[row] = board[row][col];
        }

        vector<int> old = temp;

        compress(temp);
        merge(temp);
        compress(temp);

        if (temp != old) {
            changed = true;
        }

        for (int row = 0; row < SIZE; row++) {
            board[row][col] = temp[row];
        }
    }

    return changed;
}

bool moveDown() {
    bool changed = false;

    for (int col = 0; col < SIZE; col++) {

        vector<int> temp(SIZE);

        for (int row = 0; row < SIZE; row++) {
            temp[row] = board[row][col];
        }

        reverseRow(temp);

        vector<int> old = temp;

        compress(temp);
        merge(temp);
        compress(temp);

        reverseRow(temp);

        if (board[0][col] != temp[0] ||
            board[1][col] != temp[1] ||
            board[2][col] != temp[2] ||
            board[3][col] != temp[3]) {
            changed = true;
        }

        for (int row = 0; row < SIZE; row++) {
            board[row][col] = temp[row];
        }
    }

    return changed;
}

bool canMove() {

    for (int i = 0; i < SIZE; i++) {

        for (int j = 0; j < SIZE; j++) {

            if (board[i][j] == 0)
                return true;

            if (j < SIZE - 1 &&
                board[i][j] == board[i][j + 1])
                return true;

            if (i < SIZE - 1 &&
                board[i][j] == board[i + 1][j])
                return true;
        }
    }

    return false;
}

char getInput() {

    char c = getchar();

    if (c == 27) {

        getchar();

        switch (getchar()) {

            case 'A':
                return 'w';

            case 'B':
                return 's';

            case 'C':
                return 'd';

            case 'D':
                return 'a';
        }
    }

    return c;
}

int main() {

    enableRawMode();

    initBoard();

    while (true) {

        printBoard();

        if (!canMove()) {

            cout << "\nGame Over!\n";
            cout << "Final Score: " << score << endl;

            break;
        }

        char input = getInput();

        bool moved = false;

        switch (input) {

            case 'a':
                moved = moveLeft();
                break;

            case 'd':
                moved = moveRight();
                break;

            case 'w':
                moved = moveUp();
                break;

            case 's':
                moved = moveDown();
                break;

            case 'q':
                return 0;
        }

        if (moved) {
            addRandomTile();
        }
    }

    return 0;
}
