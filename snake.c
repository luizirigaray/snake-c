#include <ncurses.h>

#define KEY_ESC 27

#define DELAY_MS 100

int main(void) {
    initscr();
    set_escdelay(25);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(DELAY_MS);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int y = rows/2;
    int x = cols/2;
    int dy = 0;
    int dx = 1;

    while(1){
        erase();
        mvprintw(0, 0, "Setas mudam a direcao, esc termina | y=%d y=%d", y, x);
        mvaddch(y, x, '0');
        refresh();

        int ch = getch();
        if (ch == KEY_ESC){
            break;
        }
    

        switch (ch){
            case KEY_UP: dy = -1; dx = 0; break;
            case KEY_DOWN: dy = 1; dx = 0; break;
            case KEY_LEFT: dy = 0; dx = -1; break;
            case KEY_RIGHT: dy = 0; dx = 1; break;
        }

        int ny = y + dy;
        int nx = x + dx;
        if(ny >= 1 && ny < rows && nx >= 0 && nx < cols){
            y = ny;
            x = nx;
        }
    }
    
    endwin();
    return 0;
}