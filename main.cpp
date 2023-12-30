#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

bool gameover;
int x, y, fruitX, fruitY, score, speed;

const int width = 20;
const int height = 15;

const int maxTail = width * height;
int tailX[maxTail], tailY[maxTail];
int tailCount;

enum Direction {
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
Direction dir;

void Setup() {
    srand(time(0));
    gameover = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    speed = 150;
}

void Draw() {
    // update console display
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0});

    // draw top border
    for (int i = 0; i < width+2; i++) {
        cout << "#"; 
    }
    cout << endl;

    for (int i = 0; i < height; i++) {
        cout << "#"; // draw left border
        for (int j = 0; j < width; j++) {
            if (i == y && j == x) {
                cout << "O"; // draw snake head
            } else if (i == fruitY && j == fruitX) {
                cout << "X"; // draw fruit
            } else {
                bool printTail = false;
                for (int k = 0; k < tailCount; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o"; // draw snake tail
                        printTail = true;
                    }
                }
                if (!printTail) {
                    cout << " ";
                }
            }
        }
        cout << "#"; // draw right border
        cout << endl;
    }

    // draw bottom border
    for (int i = 0; i < width+2; i++) {
        cout << "#"; 
    }
    cout << endl;

    // display score
    cout << "Score: " << score << endl;

    cout << "Press 'c' to pause and 'x' to exit." << endl;
}

void Input() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 'w':
            case 72: // up arrow key
                if (dir != DOWN) {
                    dir = UP;
                }
                break;
            case 'a':
            case 75: // left arrow key
                if (dir != RIGHT) {
                    dir = LEFT;
                }
                break;
            case 's':
            case 80: // down arrow key
                if (dir != UP) {
                    dir = DOWN;
                }
                break;
            case 'd':
            case 77: // right arrow key
                if (dir != LEFT) {
                    dir = RIGHT;
                }
                break;
            case 'c':
                dir = STOP; // pause game
                break;
            case 'x':
                gameover = true; // exit game
                break;
        }
    }
}

void Logic() {
    // snake tail
    for (int i = tailCount; i >= 0; i--) {
        tailX[i] = tailX[i - 1];
        tailY[i] = tailY[i - 1];
    }
    tailX[0] = x;
    tailY[0] = y;

    // snake direction
    switch(dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    // pass through wall
    if (x >= width) x = 0; else if (x < 0) x = width-1;
    if (y >= height) y = 0; else if (y < 0) y = height-1;

    // eat fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        tailCount++;

        // generate new fruit
        bool fruitOnSnake = true;
        while (fruitOnSnake) {
            fruitX = rand() % width;
            fruitY = rand() % height;

            fruitOnSnake = false;
            for (int i = 0; i < tailCount; i++) {
                if (tailX[i] == fruitX && tailY[i] == fruitY) {
                    fruitOnSnake = true;
                    break;
                }
            }
        }

        // speed up
        if (speed > 35) {
            speed -= 5;
        }
    }

    // game over if tail touched
    for (int i = 0; i < tailCount; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameover = true;
        }
    }
}

void HideConsoleCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

int main() {
    system("cls");
    HideConsoleCursor(); 
    Setup();
    while (!gameover) {
        Draw();
        Input();
        if (dir != STOP) {
            Logic(); // pause game
        }
        Sleep(speed);
    }
    if (gameover) {
        cout << "Game over!";
    }
    return 0;
}