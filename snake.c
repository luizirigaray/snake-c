#include <ncurses.h>

#define KEY_ESC 27

int main(void) {
    initscr();
    set_escdelay(25);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int y = rows/2;
    int x = cols/2;

    while(1){
        erase();
        mvprintw(0, 0, "Setas movem, esc termina");
        mvaddch(y, x, '0');
        refresh();

        int ch = getch();
        if (ch == KEY_ESC){
            break;
        }
    

        switch (ch){
            case KEY_UP: if (y > 1) y--; break;
            case KEY_DOWN: if (y < rows -1) y++; break;
            case KEY_LEFT: if (x > 0) x--; break;
            case KEY_RIGHT: if (x < cols -1) x++; break;
        }
    }
    
    endwin();
    return 0;
}