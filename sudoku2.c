#include <stdio.h>
#include <unistd.h>
#define DIM 9
#define SQUARE_DIM 3

int is_valid(int grid[DIM][DIM], int row, int col, int num) {
    for (int i = 0; i < DIM; i++) {
        if (grid[row][i] == num ||
            grid[i][col] == num) { // checking rows and cols
            return 0;
        }
    }

    // checking the square
    int start_row = row - row % SQUARE_DIM;
    int start_col = col - col % SQUARE_DIM;

    for (int i = 0; i < SQUARE_DIM; i++) {
        for (int j = 0; j < SQUARE_DIM; j++) {
            if (grid[start_row + i][start_col + j] == num) {
                return 0;
            }
        }
    }
    return 1;
}

void update_cell(
    int row, int col, int value, const char *color, useconds_t delay) {
    int screen_row = row + 1;
    int screen_col = col * 4 + 1;

    printf("\033[%d;%dH%s%d\033[0m", screen_row, screen_col, color, value);
    fflush(stdout);
    usleep(delay);
}

int solve(int grid[DIM][DIM], int row, int col) {
    int next_row, next_col;

    if (row == DIM)
        return 1;

    next_row = (col == DIM - 1) ? row + 1 : row;
    next_col = (col == DIM - 1) ? 0 : col + 1;

    if (grid[row][col] != 0) {
        update_cell(row, col, grid[row][col], "\033[37m",
                    50000); // white for fixed cells
        return solve(grid, next_row, next_col);
    } else {
        for (int num = 1; num <= DIM; num++) {
            if (is_valid(grid, row, col, num)) {
                grid[row][col] = num;

                update_cell(row, col, num, "\033[32m",
                            70000); // green for valid cells

                if (solve(grid, next_row, next_col)) {
                    return 1;
                }
                grid[row][col] = 0; // backtracking
                update_cell(row, col, 0, "\033[31m",
                            150000); // red for backtracking
            }
        }
        return 0; // triggers bakctracking
    }
}

int main() {

    int grid[DIM][DIM] = {
        {0, 0, 0, 2, 6, 0, 7, 0, 1}, {6, 8, 0, 0, 7, 0, 0, 9, 0},
        {1, 9, 0, 0, 0, 4, 5, 0, 0}, {8, 2, 0, 1, 0, 0, 0, 4, 0},
        {0, 0, 4, 6, 0, 2, 9, 0, 0}, {0, 5, 0, 0, 0, 3, 0, 2, 8},
        {0, 0, 9, 3, 0, 0, 0, 7, 4}, {0, 4, 0, 0, 5, 0, 0, 3, 6},
        {7, 0, 3, 0, 1, 8, 0, 0, 0}};

    printf("\e[1;1H\e[2J"); // to clear the screen

    solve(grid, 0, 0);
}
