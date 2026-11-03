#ifndef SNAKE_H
#define SNAKE_H

#define MAX_LEN 100
#define INITIAL_LEN 5
#define DELAY_MS 100
#define MIN_DELAY_MS 40

typedef struct {
    int y;
    int x;
} Point;

typedef struct {
    Point body[MAX_LEN];
    int len;
    int dy, dx;
    Point food;
    int score;
    int rows, cols;
} Game;

typedef enum {
    MOVE_OK,
    MOVE_ATE,
    MOVE_DEAD
} MoveResult;

void game_init(Game *g, int rows, int cols);
void game_set_direction(Game *g, int dy, int dx);
MoveResult game_step(Game *g);
int game_delay_ms(const Game *g);

#endif