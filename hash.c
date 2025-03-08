#include "hash.h"

#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

union table {
    void *matrix[TABLE_DIMENSION_Y][TABLE_DIMENSION_X];
    void *array[TABLE_SIZE];
};

int8_t hash(char c) {
    assert(c >= 'A' && c <= 'Z');

    return c - 'A';
}

void table_set(Table *t, char *index, void *obj) {
    int8_t y = hash(index[0]);
    int8_t x = hash(index[1]);

    t->matrix[y][x] = obj;
}

void *table_get(Table *t, char *index) {
    int8_t y = hash(index[0]);
    int8_t x = hash(index[1]);

    return t->matrix[y][x];
}

Table *new_table(Table *old) {
    Table *new = calloc(1, sizeof(Table));

    if (old) {
	*new = *old;
    }

    return new;
}
