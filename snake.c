#include <ncurses.h>
#include <string.h>

int main(void) {
    initscr();
    noecho();
    curs_set(0);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    const char *msg = "Ola, Snake!";
    mvprintw(rows / 2, (cols - strlen(msg)) / 2, "%s", msg);
    refresh();

    getch();
    endwin();
    return 0;
}