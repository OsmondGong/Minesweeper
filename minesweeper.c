// Assignment 1 20T1 COMP1511: Minesweeper
// minesweeper.c
//
// This program was written by Osmond Gong (z5293467)
// on 15/03/2020
//
// Version 1.0.0 (2020-03-08): Assignment released.
// Version 1.0.1 (2020-03-08): Fix punctuation in comment.
// Version 1.0.2 (2020-03-08): Fix second line of header comment to say minesweeper.c

#include <stdio.h>
#include <stdlib.h>

// Possible square states.
#define VISIBLE_SAFE    0
#define HIDDEN_SAFE     1
#define HIDDEN_MINE     2

// The size of the starting grid.
#define SIZE 8

// The possible command codes.
#define DETECT_ROW              1
#define DETECT_COL              2
#define DETECT_SQUARE           3
#define REVEAL_SQUARE           4
#define GAMEPLAY_MODE           5
#define DEBUG_MODE              6
#define REVEAL_RADIAL           7

// Add any extra #defines here.

void initialise_field(int minefield[SIZE][SIZE]);
void print_debug_minefield(int minefield[SIZE][SIZE]);

// Place your function prototyes here.


int main(void) {
    int minefield[SIZE][SIZE];
    initialise_field(minefield);
    printf("Welcome to minesweeper!\n");
    printf("How many mines? ");

    // TODO: Scan in the number of pairs of mines.
    int mines;
    scanf("%d", &mines);

    printf("Enter pairs:\n");

    // TODO: Scan in the pairs of mines and place them on the grid.
    int coord = 1;
    while (coord <= mines) {
        int ycoord, xcoord;
        scanf("%d", &ycoord);
        scanf("%d", &xcoord);
        if (-1 < ycoord && ycoord < SIZE && -1 < xcoord && xcoord < SIZE) {
            minefield[ycoord][xcoord] = 2;
        }
        coord++;
    }

    printf("Game Started\n");
    print_debug_minefield(minefield);

    // TODO: Scan in commands to play the game until the game ends.
    int win = 0;
    while (win == 0) {
        int command = 0;
        scanf("%d", &command);
        if (command > 0) {
            if (command == DETECT_ROW) {
                int row;
                scanf("%d", &row);
                int col = 0;
                int num = 0;
                while (col < SIZE) {
                    if (minefield[row][col] == HIDDEN_MINE) {
                        num++;
                    }
                    col++;
                }
                printf("There are %d mine(s) in row %d\n", num, row);
            }
            else if (command == DETECT_COL) {
                int col;
                scanf("%d", &col);
                int row = 0;
                int num = 0;
                while (row < SIZE) {
                    if (minefield[row][col] == HIDDEN_MINE) {
                        num++;
                    }
                    row++;
                }
                printf("There are %d mine(s) in column %d\n", num, col);
            }
            else if (command == DETECT_SQUARE) {
                int row, col, size, row_size, col_size;

                scanf("%d", &row);
                scanf("%d", &col);
                scanf("%d", &size);

                int mine_count = 0;

                if (size % 2 == 0 && size > 0) {
                    size--;
                }
                
                int col_edge = 0;
                if (col == 0) {
                    col_size = size / 2 + 1;
                    col_edge = 1;
                }
                else if (col == 7) {
                    col_size = size / 2 + 1;
                    col = col - size / 2;
                }
                else {
                    col = col - size / 2;
                    col_size = size;
                }

                int row_edge = 0;
                if (row == 0) {
                    row_size = size / 2 + 1;
                    row_edge = 1;
                }                
                else if (row == 7) {
                    row_size = size / 2 + 1;
                    row = row - size / 2;
                }
                else {
                    row = row - size / 2;
                    row_size = size;
                }

                if (size >= 0) {
                    int i = 0;
                    while (i < row_size) {
                        int j = 0;
                        while (j < col_size){
                            if (minefield[row][col] == HIDDEN_MINE) {
                                mine_count++;
                            }
                            j++;
                            col++;
                        }
                        col = col - col_size;
                        i++;
                        row++;
                    }
                    row = row - row_size;
                    if (row_edge == 0) {
                        row = row + size/2;
                    }
                    if (col_edge == 0) {
                        col = col + size/2;
                    }
                }
                printf("There are %d mine(s) in the square centered at row %d, column %d of size %d\n", mine_count, row, col, size);
            }
            else if (command == REVEAL_SQUARE) {
                int row, col, col_size, row_size;

                scanf("%d", &row);
                scanf("%d", &col);

                int size = 3;
                
                if (minefield[row][col] == HIDDEN_MINE) {
                    win = -1;
                    printf ("Game over\n");
                }
                
                int col_edge = 0;
                if (col == 0) {
                    col_size = size / 2 + 1;
                    col_edge = 1;
                }
                else if (col == 7) {
                    col_size = size / 2 + 1;
                    col = col - size / 2;
                }
                else {
                    col = col - size / 2;
                    col_size = size;
                }

                int row_edge = 0;
                if (row == 0) {
                    row_size = size / 2 + 1;
                    row_edge = 1;
                }                
                else if (row == 7) {
                    row_size = size / 2 + 1;
                    row = row - size / 2;
                }
                else {
                    row = row - size / 2;
                    row_size = size;
                }

                int scenario = 0;

                int i = 0;
                while (i < row_size) {
                    int j = 0;
                    while (j < col_size){
                        if (minefield[row][col] == HIDDEN_MINE) {
                            scenario = 1;
                        }
                        j++;
                        col++;
                    }
                    col = col - col_size;
                    i++;
                    row++;
                }
                row = row - row_size;

                if (scenario == 1) {
                    if (row_edge == 0) {
                        row = row + size/2;
                    }

                    if (col_edge == 0) {
                        col = col + size/2;
                    }
                    if (minefield[row][col] == HIDDEN_SAFE){
                        minefield[row][col] = VISIBLE_SAFE;
                    }
                }

                else {
                    i = 0;
                    while (i < row_size) {
                        int j = 0;
                        while (j < col_size){
                            minefield[row][col] = VISIBLE_SAFE;
                            j++;
                            col++;
                        }
                        col = col - col_size;
                        i++;
                        row++;
                    }
                    row = row - row_size;

                    if (row_edge == 0) {
                        row = row + size/2;
                    }
                    if (col_edge == 0) {
                        col = col + size/2;
                    }
                }
                int win_check = 1;
                int k = 0;
                while (k < SIZE) {
                    int l = 0;
                    while (l < SIZE){
                        if (minefield[k][l] == HIDDEN_SAFE) {
                            win_check = 0;
                        }
                        l++;
                    }
                    k++;
                }
                if (win_check == 1) {
                    printf("Game Won!\n");
                    win = 1;
                }
            }
            print_debug_minefield(minefield);
        }
        else {
            win = 1;
        }

    }
    // A game ends when the player wins, loses, or enters EOF (Ctrl+D).
    // You should display the minefield after each command has been processed.

    return 0;
}

// Set the entire minefield to HIDDEN_SAFE.
void initialise_field(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            minefield[i][j] = HIDDEN_SAFE;
            j++;
        }
        i++;
    }
}

// Print out the actual values of the minefield.
void print_debug_minefield(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            printf("%d ", minefield[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}