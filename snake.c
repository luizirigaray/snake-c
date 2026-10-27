#include <ncurses.h>

#define KEY_ESC 27
#define DELAY_MS 100
#define MAX_LEN 100

typedef struct {
    int y;
    int x;
} Point;

int main(void) {
    initscr();
    set_escdelay(25);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(DELAY_MS);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    Point snake[MAX_LEN];
    int len = 5;

    for (int i = 0; i < len; i++) {
        snake[i].y = rows / 2;
        snake[i].x = cols / 2 - i;
    }

    int dy = 0;
    int dx = 1;

    while (1) {
        erase();
        mvprintw(0, 0, "Setas mudam a direcao, Esc sai | len=%d", len);

        for (int i = len - 1; i >= 0; i--) {
            char c = (i == 0) ? '@' : 'o';
            mvaddch(snake[i].y, snake[i].x, c);
        }
        refresh();

        int ch = getch();
        if (ch == KEY_ESC) {
            break;
        }

        switch (ch) {
            case KEY_UP:    dy = -1; dx = 0;  break;
            case KEY_DOWN:  dy = 1;  dx = 0;  break;
            case KEY_LEFT:  dy = 0;  dx = -1; break;
            case KEY_RIGHT: dy = 0;  dx = 1;  break;
        }

        int ny = snake[0].y + dy;
        int nx = snake[0].x + dx;
        if (ny >= 1 && ny < rows && nx >= 0 && nx < cols) {
            snake[0].y = ny;
            snake[0].x = nx;
        }
    }

    endwin();
    return 0;
}