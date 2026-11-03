#include <stdlib.h>
#include <string.h>
#include "snake.h"

static void spawn_food(Game *g) {
    int ok;
    do {
        g->food.y = 1 + rand() % (g->rows - 1);
        g->food.x = rand() % g->cols;
        ok = 1;
        for (int i = 0; i < g->len; i++) {
            if (g->body[i].y == g->food.y && g->body[i].x == g->food.x) {
                ok = 0;
                break;
            }
        }
    } while (!ok);
}

void game_init(Game *g, int rows, int cols) {
    g->rows = rows;
    g->cols = cols;
    g->len = INITIAL_LEN;
    for (int i = 0; i < g->len; i++) {
        g->body[i].y = rows / 2;
        g->body[i].x = cols / 2 - i;
    }
    g->dy = 0;
    g->dx = 1;
    g->score = 0;
    spawn_food(g);
}

void game_set_direction(Game *g, int dy, int dx) {
    if (dy == -g->dy && dx == -g->dx) {
        return;   // proibe inverter 180 graus
    }
    g->dy = dy;
    g->dx = dx;
}

MoveResult game_step(Game *g) {
    int ny = g->body[0].y + g->dy;
    int nx = g->body[0].x + g->dx;

    if (ny < 1 || ny >= g->rows || nx < 0 || nx >= g->cols) {
        return MOVE_DEAD;
    }

    for (int i = 0; i < g->len - 1; i++) {
        if (g->body[i].y == ny && g->body[i].x == nx) {
            return MOVE_DEAD;
        }
    }

    int ate = (ny == g->food.y && nx == g->food.x);
    if (ate && g->len < MAX_LEN) {
        g->len++;
    }

    memmove(&g->body[1], &g->body[0], (g->len - 1) * sizeof(Point));
    g->body[0].y = ny;
    g->body[0].x = nx;

    if (ate) {
        g->score += 10;
        spawn_food(g);
        return MOVE_ATE;
    }
    return MOVE_OK;
}

int game_delay_ms(const Game *g) {
    int delay = DELAY_MS - (g->len - INITIAL_LEN) * 5;
    return delay < MIN_DELAY_MS ? MIN_DELAY_MS : delay;
}