#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KEY_ESC 27
#define DELAY_MS 100
#define MIN_DELAY_MS 40
#define MAX_LEN 100
#define INITIAL_LEN 5

typedef struct {
    int y;
    int x;
} Point;

static void spawn_food(Point *food, const Point *snake, int len, int rows, int cols) {
    int ok;
    do {
        food->y = 1 + rand() % (rows - 1);
        food->x = rand() % cols;
        ok = 1;
        for (int i = 0; i < len; i++) {
            if (snake[i].y == food->y && snake[i].x == food->x) {
                ok = 0;
                break;
            }
        }
    } while (!ok);
}

int main(void) {
    initscr();
    set_escdelay(25);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    srand(time(NULL));

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int quit = 0;

    while (!quit) {
        // ---- estado inicial de uma partida ----
        Point snake[MAX_LEN];
        int len = INITIAL_LEN;
        for (int i = 0; i < len; i++) {
            snake[i].y = rows / 2;
            snake[i].x = cols / 2 - i;
        }

        int dy = 0;
        int dx = 1;
        int score = 0;
        int paused = 0;

        Point food;
        spawn_food(&food, snake, len, rows, cols);

        // ---- loop da partida ----
        while (1) {
            int delay = DELAY_MS - (len - INITIAL_LEN) * 5;
            if (delay < MIN_DELAY_MS) {
                delay = MIN_DELAY_MS;
            }
            timeout(delay);

            erase();
            mvprintw(0, 0, "Pontos: %d | Tamanho: %d | p pausa, Esc sai", score, len);
            mvaddch(food.y, food.x, '*');
            for (int i = len - 1; i >= 0; i--) {
                char c = (i == 0) ? '@' : 'o';
                mvaddch(snake[i].y, snake[i].x, c);
            }
            if (paused) {
                mvprintw(rows / 2, (cols - 7) / 2, "PAUSADO");
            }
            refresh();

            int ch = getch();
            if (ch == KEY_ESC) {
                quit = 1;
                break;
            }
            if (ch == 'p') {
                paused = !paused;
            }
            if (paused) {
                continue;
            }

            switch (ch) {
                case KEY_UP:    if (dy != 1)  { dy = -1; dx = 0; } break;
                case KEY_DOWN:  if (dy != -1) { dy = 1;  dx = 0; } break;
                case KEY_LEFT:  if (dx != 1)  { dy = 0;  dx = -1; } break;
                case KEY_RIGHT: if (dx != -1) { dy = 0;  dx = 1; } break;
            }

            int ny = snake[0].y + dy;
            int nx = snake[0].x + dx;

            if (ny < 1 || ny >= rows || nx < 0 || nx >= cols) {
                break;
            }

            int bateu = 0;
            for (int i = 0; i < len - 1; i++) {
                if (snake[i].y == ny && snake[i].x == nx) {
                    bateu = 1;
                    break;
                }
            }
            if (bateu) {
                break;
            }

            int comeu = (ny == food.y && nx == food.x);
            if (comeu && len < MAX_LEN) {
                len++;
            }

            memmove(&snake[1], &snake[0], (len - 1) * sizeof(Point));
            snake[0].y = ny;
            snake[0].x = nx;

            if (comeu) {
                score += 10;
                spawn_food(&food, snake, len, rows, cols);
            }
        }

        if (quit) {
            break;
        }

        // ---- tela de fim de jogo ----
        erase();
        mvprintw(rows / 2 - 1, (cols - 9) / 2, "GAME OVER");
        mvprintw(rows / 2, (cols - 12) / 2, "Pontos: %d", score);
        mvprintw(rows / 2 + 2, (cols - 24) / 2, "r = jogar de novo");
        mvprintw(rows / 2 + 3, (cols - 24) / 2, "Esc = sair");
        refresh();

        timeout(-1);
        int k;
        do {
            k = getch();
        } while (k != 'r' && k != KEY_ESC);

        if (k == KEY_ESC) {
            quit = 1;
        }
    }

    endwin();
    return 0;
}