#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "snake.h"

#define KEY_ESC 27

static void draw(const Game *g, int paused) {
    erase();
    mvprintw(0, 0, "Pontos: %d | Tamanho: %d | p pausa, Esc sai", g->score, g->len);
    mvaddch(g->food.y, g->food.x, '*');
    for (int i = g->len - 1; i >= 0; i--) {
        char c = (i == 0) ? '@' : 'o';
        mvaddch(g->body[i].y, g->body[i].x, c);
    }
    if (paused) {
        mvprintw(g->rows / 2, (g->cols - 7) / 2, "PAUSADO");
    }
    refresh();
}

static void draw_game_over(const Game *g) {
    erase();
    mvprintw(g->rows / 2 - 1, (g->cols - 9) / 2, "GAME OVER");
    mvprintw(g->rows / 2, (g->cols - 12) / 2, "Pontos: %d", g->score);
    mvprintw(g->rows / 2 + 2, (g->cols - 17) / 2, "r = jogar de novo");
    mvprintw(g->rows / 2 + 3, (g->cols - 10) / 2, "Esc = sair");
    refresh();
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

    Game game;
    int quit = 0;

    while (!quit) {
        game_init(&game, rows, cols);
        int paused = 0;

        while (1) {
            timeout(game_delay_ms(&game));
            draw(&game, paused);

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
                case KEY_UP:    game_set_direction(&game, -1, 0); break;
                case KEY_DOWN:  game_set_direction(&game, 1, 0);  break;
                case KEY_LEFT:  game_set_direction(&game, 0, -1); break;
                case KEY_RIGHT: game_set_direction(&game, 0, 1);  break;
            }

            if (game_step(&game) == MOVE_DEAD) {
                break;
            }
        }

        if (quit) {
            break;
        }

        draw_game_over(&game);
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