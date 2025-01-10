#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_INT_TYPE int

#define GRID_Y_SIZE 3
#define GRID_X_SIZE 7

typedef struct vector {
    VECTOR_INT_TYPE y;
    VECTOR_INT_TYPE x;
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

void init_grid_vec(vec_t *grid_vec, win_t *body) {
    grid_vec->y = body->vec.y / GRID_Y_SIZE;
    grid_vec->x = body->vec.x / GRID_X_SIZE;
}

void init_grid(WINDOW ****grid, vec_t *grid_vec, win_t *title, win_t *body) {
    *grid = calloc(grid_vec->y, sizeof(WINDOW**));

    for (int y = 0; y < grid_vec->y; y++) {
	(*grid)[y] = calloc(grid_vec->x, sizeof(WINDOW*));

	for (int x = 0; x < grid_vec->x; x++) {
	    (*grid)[y][x] = subwin(body->win, GRID_Y_SIZE, GRID_X_SIZE, y * GRID_Y_SIZE + title->vec.y + (body->vec.y % GRID_Y_SIZE) / 2, x * GRID_X_SIZE + (body->vec.x % GRID_X_SIZE) / 2);
	}
    }
}

void print_init_title(win_t *title, const char *str) {
    box(title->win, 0, 0);

    mvwprintw(title->win, 1, (title->vec.x - strlen(str)) / 2, str);
}

void print_init_grid(WINDOW ****grid, vec_t *grid_vec) {
    for (int y = 0; y < grid_vec->y; y++) {
	for (int x = 0; x < grid_vec->x; x++) {
	    box((*grid)[y][x], 0, 0);
	    mvwprintw((*grid)[y][x], 1, 2, "%c%d", x + 'A', y + 1);
	}
    }
}

int main(void) {
    win_t title, body;
    vec_t grid_vec;
    WINDOW ***grid;

    setlocale(LC_ALL, "");

    initscr();

    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    init_title(&title);
    init_body(&body, &title);
    init_grid_vec(&grid_vec, &body);
    init_grid(&grid, &grid_vec, &title, &body);

    print_init_title(&title, "shoosh");
    print_init_grid(&grid, &grid_vec);

    refresh();
    wrefresh(title.win);
    wrefresh(body.win);

    while (true) {
	int ch = getch();

	if (ch == 'q') break;
    }

    for (int y = 0; y < grid_vec.y; y++) {
	for (int x = 0; x < grid_vec.x; x++) {
	    delwin(grid[y][x]);
	}

	free(grid[y]);
    }

    free(grid);

    delwin(body.win);
    delwin(title.win);
    endwin();
}
