#include <stdio.h>
#include <stdlib.h>
#define NUM_ROWS 60
#define BLUE "\x1B[36m"
struct cell {
    int row;
    int col;
    int alive;
};

void setup_grid(struct cell **, int);
void print_grid(struct cell **, int);
void teardown_grid(struct cell **, int);
void determine_next_gen(struct cell **);
struct cell ** copy_grid(struct cell **);
int legal_pos(int, int);
int main(void) {
    int i = 0;
    struct cell ** grid = (struct cell **) malloc(NUM_ROWS * sizeof(struct cell *));
    setup_grid(grid, NUM_ROWS);
    while (i < 10000) {
      print_grid(grid, NUM_ROWS);
      determine_next_gen(grid);
      i++;
    }
    teardown_grid(grid, NUM_ROWS);

    return 0;
}

struct cell ** copy_grid(struct cell ** grid) {
    struct cell ** copy = (struct cell **) malloc(NUM_ROWS * sizeof(struct cell *));
    for (int i = 0; i < NUM_ROWS; i++) {
        copy[i] = (struct cell *) malloc(sizeof(struct cell) * NUM_ROWS); 
        for (int j = 0; j < NUM_ROWS; j++) {
            copy[i][j].row = grid[i][j].row;
            copy[i][j].col = grid[i][j].col;
            copy[i][j].alive = grid[i][j].alive;
        }
    }
    return copy;
}

int legal_pos(int row, int col) {
    if (row >= 0 && row < NUM_ROWS && col >= 0 && col < NUM_ROWS) return 1;
    return 0;
}

void determine_next_gen(struct cell ** grid) {
    struct cell ** copy = copy_grid(grid);
    //do work
    int deltas[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_ROWS; j++) {
            int total_living_neighbors = 0;
            // todo refactor this to own fn
            for (int k = 0; k < 8; k++) {
                int neighbor_row = i + deltas[k][0];
                int neighbor_col = j + deltas[k][1];
                if (legal_pos(neighbor_row, neighbor_col)) {
                    if (copy[neighbor_row][neighbor_col].alive) {
                        total_living_neighbors++;
                    }
                }
            }
            if (grid[i][j].alive) {
                if (total_living_neighbors < 2 || total_living_neighbors > 3) {
                    grid[i][j].alive = 0;
                }
            } else {
                if (total_living_neighbors == 3) {
                    grid[i][j].alive = 1;
                }
            }
            total_living_neighbors = 0;
        }
    }
    free(copy);
}

void setup_grid(struct cell ** grid, int len) {
    for (int i = 0; i < len; i++) {
        grid[i] = (struct cell *) malloc(sizeof(struct cell) * len);
        for (int j = 0; j < len; j++) {
            grid[i][j].row = i;
            grid[i][j].col = j;
            grid[i][j].alive = rand() % 2;
        }
    }
}

void print_grid(struct cell ** grid, int len) {
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            if (grid[i][j].alive) {
                printf("%s $ ", BLUE);
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }
}

void teardown_grid(struct cell ** grid, int len) {
    for (int i = 0; i < len; i++) {
        free(grid[i]);
    }
    free(grid);
} 