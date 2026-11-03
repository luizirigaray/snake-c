#include <stdio.h>
#include "snake.h"

static int failures = 0;

#define CHECK(cond)                                              \
    do {                                                         \
        if (!(cond)) {                                           \
            printf("FALHOU: %s (linha %d)\n", #cond, __LINE__);  \
            failures++;                                          \
        }                                                        \
    } while (0)

static void test_init(void) {
    Game g;
    game_init(&g, 24, 80);
    CHECK(g.len == INITIAL_LEN);
    CHECK(g.score == 0);
    CHECK(g.body[0].y == 12);
    CHECK(g.body[0].x == 40);
    CHECK(g.dy == 0 && g.dx == 1);
}

static void test_move(void) {
    Game g;
    game_init(&g, 24, 80);
    g.food = (Point){1, 1};   // comida longe do caminho
    Point old_head = g.body[0];

    CHECK(game_step(&g) == MOVE_OK);
    CHECK(g.body[0].x == old_head.x + 1);
    CHECK(g.body[1].x == old_head.x);
    CHECK(g.len == INITIAL_LEN);
}

static void test_wall(void) {
    Game g;
    game_init(&g, 24, 80);
    g.food = (Point){1, 1};
    g.body[0].x = 79;                 // ultima coluna, indo para a direita
    CHECK(game_step(&g) == MOVE_DEAD);

    game_init(&g, 24, 80);
    g.food = (Point){5, 5};
    g.body[0].y = 1;                  // primeira linha jogavel, indo para cima
    g.dy = -1;
    g.dx = 0;
    CHECK(game_step(&g) == MOVE_DEAD);
}

static void test_eat(void) {
    Game g;
    game_init(&g, 24, 80);
    g.food = (Point){g.body[0].y, g.body[0].x + 1};   // comida logo a frente

    CHECK(game_step(&g) == MOVE_ATE);
    CHECK(g.len == INITIAL_LEN + 1);
    CHECK(g.score == 10);

    int on_snake = 0;
    for (int i = 0; i < g.len; i++) {
        if (g.body[i].y == g.food.y && g.body[i].x == g.food.x) {
            on_snake = 1;
        }
    }
    CHECK(!on_snake);   // a nova comida nao nasce em cima da cobra
}

static void test_self_collision(void) {
    Game g;
    game_init(&g, 24, 80);
    g.food = (Point){1, 1};
    g.len = 5;
    g.body[0] = (Point){10, 10};
    g.body[1] = (Point){11, 10};
    g.body[2] = (Point){11, 9};
    g.body[3] = (Point){10, 9};
    g.body[4] = (Point){9, 9};
    g.dy = 0;
    g.dx = -1;   // a cabeca vai para (10, 9), que e o corpo
    CHECK(game_step(&g) == MOVE_DEAD);
}

static void test_tail_cell_is_free(void) {
    Game g;
    game_init(&g, 24, 80);
    g.food = (Point){1, 1};
    g.len = 4;
    g.body[0] = (Point){10, 10};
    g.body[1] = (Point){11, 10};
    g.body[2] = (Point){11, 11};
    g.body[3] = (Point){10, 11};
    g.dy = 0;
    g.dx = 1;   // a cabeca vai para (10, 11), onde esta a cauda
    CHECK(game_step(&g) == MOVE_OK);   // a cauda sai dali no mesmo tique
}

static void test_no_reverse(void) {
    Game g;
    game_init(&g, 24, 80);
    game_set_direction(&g, 0, -1);   // tenta inverter 180 graus
    CHECK(g.dx == 1);
    game_set_direction(&g, -1, 0);   // virar para cima e permitido
    CHECK(g.dy == -1 && g.dx == 0);
}

static void test_delay(void) {
    Game g;
    game_init(&g, 24, 80);
    CHECK(game_delay_ms(&g) == DELAY_MS);
    g.len = MAX_LEN;
    CHECK(game_delay_ms(&g) == MIN_DELAY_MS);
}

int main(void) {
    test_init();
    test_move();
    test_wall();
    test_eat();
    test_self_collision();
    test_tail_cell_is_free();
    test_no_reverse();
    test_delay();

    if (failures == 0) {
        printf("Todos os testes passaram.\n");
        return 0;
    }
    printf("%d teste(s) falharam.\n", failures);
    return 1;
}