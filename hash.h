#pragma once

#define TABLE_DIMENSION_Y 26
#define TABLE_DIMENSION_X 26
#define TABLE_SIZE TABLE_DIMENSION_Y * TABLE_DIMENSION_X

typedef union table Table;

void table_set(Table*, char *index, void *obj);
void *table_get(Table*, char *index);

Table *new_table(Table *old);
