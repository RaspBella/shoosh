#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

#define GRID_DIM_Y 26
#define GRID_DIM_X 26
#define ATLEAST_SIZE 8

typedef struct vector {
    int y;
    int x;
} vec_t;

typedef struct window {
    vec_t vec;
    WINDOW *win;
} win_t;

void init_title(win_t *title) {
    title->vec.y = 3;
    title->vec.x = COLS;

    title->win = newwin(title->vec.y, title->vec.x, 0, 0);
}

void init_body(win_t *body, win_t *title) {
    body->vec.y = LINES - title->vec.y;
    body->vec.x = COLS;
    
    body->win = newwin(body->vec.y, body->vec.x, title->vec.y, 0);
}

void init_grid(WINDOW *grid[GRID_DIM_Y][GRID_DIM_X], win_t *body) {
    for (int y = 0; y < GRID_DIM_Y; y++) {
	for (int x = 0; x < GRID_DIM_X; x++) {
	    int rows = body->vec.y / GRID_DIM_Y;
	    int cols = body->vec.x / GRID_DIM_X;

	    if (rows < ATLEAST_SIZE) rows = ATLEAST_SIZE;
	    if (cols < ATLEAST_SIZE) cols = ATLEAST_SIZE;

	    int start_y = y * rows;
	    int start_x = x * cols;

	    grid[y][x] = derwin(body->win, rows, cols, start_y, start_x);
	}
    }
}

void print_title(win_t *title, const char *str) {
    box(title->win, 0, 0);

    mvwprintw(title->win, 1, (title->vec.x - strlen(str)) / 2, str);
}

void print_grid(WINDOW *grid[GRID_DIM_Y][GRID_DIM_X], Table *data, vec_t *grid_pos) {
    for (int y = 0; y < GRID_DIM_Y; y++) {
	for (int x = 0; x < GRID_DIM_X; x++) {
	    box(grid[y][x], 0, 0);

	    int real_y = y + grid_pos->y + 'A';
	    int real_x = x + grid_pos->x + 'A';

	    if (real_y < 'A' || real_y > 'Z') continue;
	    if (real_x < 'A' || real_x > 'Z') continue;

	    void *obj = table_get(data, (char[2]) { real_y, real_x });

	    if (obj) {
		mvwprintw(grid[y][x], 1, 2, "%s", (char *) obj);
	    }
	    
	    else {
		mvwprintw(grid[y][x], 1, 2, "%c%c", y + 'A', x + 'A');
	    }
	}
    }
}

int main(void) {
    win_t title, body;
    WINDOW *grid[GRID_DIM_Y][GRID_DIM_X];
    vec_t grid_pos = { 0 };
    Table *data = new_table(NULL);
    int key;



    setlocale(LC_ALL, "");

    initscr();

    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    init_title(&title);
    init_body(&body, &title);
    init_grid(grid, &body);
    
    print_title(&title, "shoosh");
    print_grid(grid, data, &grid_pos);

    refresh();
    wrefresh(title.win);
    wrefresh(body.win);



    do {
	key = getch();

	switch (key) {
	    case KEY_RESIZE: // erm
		break;

	    default:
		break;
	}
    } while (key != 'q');



    for (int y = 0; y < GRID_DIM_Y; y++) {
	for (int x = 0; x < GRID_DIM_X; x++) {
	    delwin(grid[y][x]);
	}
    }

    free(data);

    delwin(body.win);
    delwin(title.win);
    endwin();
}
