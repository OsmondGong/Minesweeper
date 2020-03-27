// Assignment 1 20T1 COMP1511: Minesweeper
// minesweeper.c
//
// This program was written by Osmond Gong (z5293467)
// on 26/03/2020
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
void DETECT_ROW_FUNCTION(int minefield[SIZE][SIZE]);
void DETECT_COL_FUNCTION(int minefield[SIZE][SIZE]);
int DETECT_SQUARE_FUNCTION(int minefield[SIZE][SIZE], int row, int col, int size, int mine_count);
void REVEAL_SQUARE_FUNCTION (int minefield[SIZE][SIZE], int row, int col);
int WIN_CHECK_FUNCTION (int minefield[SIZE][SIZE]);
void GAMEPLAY_MODE_FUNCTION (int minefield[SIZE][SIZE], int win);
void REVEAL_RADIAL_FUNCTION (int minefield[SIZE][SIZE], int row, int col);

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
    int mode = 0;
    int win = 0;
    int hint = 0;
    int turn = 1;
    while (win == 0) {
        int command = 0;
        scanf("%d", &command);
        if (command > 0) {
            if (hint == 3 && command < 4) {
                int row, col, size;
                if (command != DETECT_COL) {
                    scanf("%d", &row);
                }
                if (command != DETECT_ROW) {
                    scanf("%d", &col);
                }
                if (command == DETECT_SQUARE) {
                    scanf("%d", &size);
                }
                printf("Help already used\n");
            }

            else if (command == DETECT_ROW) {
                DETECT_ROW_FUNCTION(minefield);
                hint++;
            }

            else if (command == DETECT_COL) {
                DETECT_COL_FUNCTION(minefield);
                hint++;
            }

            else if (command == DETECT_SQUARE) {
                int row, col, size;

                scanf("%d", &row);
                scanf("%d", &col);
                scanf("%d", &size);

                int mine_count = 0;
                mine_count = DETECT_SQUARE_FUNCTION(minefield, row, col, size, mine_count);
                printf("There are %d mine(s) in the square centered at row %d, column %d of size %d\n", mine_count, row, col, size);
                hint++;
            }

            else if (command == REVEAL_SQUARE) {
                int row, col;

                scanf("%d", &row);
                scanf("%d", &col);
                
                if (minefield[row][col] == HIDDEN_MINE) {
                    if (turn == 1) {
                        while (minefield[row][col] == HIDDEN_MINE) {
                            int j = 0;
                            while (j < SIZE) {
                                int i = 7;
                                int last_row = minefield[i][j];
                                while (i > 0) {
                                    minefield[i][j] = minefield[i - 1][j];
                                    i--;
                                }
                                minefield[i][j] = last_row;
                                j++;
                            }
                        }
                        REVEAL_SQUARE_FUNCTION(minefield, row, col);
                    }
                    else {
                        win = -1;
                        printf ("Game over\n");
                    }
                }
                else {
                    REVEAL_SQUARE_FUNCTION(minefield, row, col);
                }
                turn++;
            }

            else if (command == REVEAL_RADIAL) {
                int row, col;

                scanf("%d", &row);
                scanf("%d", &col);

                if (minefield[row][col] == HIDDEN_MINE) {
                    if (turn == 1) {
                        while (minefield[row][col] == HIDDEN_MINE) {
                            int j = 0;
                            while (j < SIZE) {
                                int i = 7;
                                int last_row = minefield[i][j];
                                while (i > 0) {
                                    minefield[i][j] = minefield[i - 1][j];
                                    i--;
                                }
                                minefield[i][j] = last_row;
                                j++;
                            }
                        }
                        REVEAL_RADIAL_FUNCTION(minefield, row, col);
                    }
                    else {
                        win = -1;
                        printf ("Game over\n");
                    }
                }
                else {
                    REVEAL_RADIAL_FUNCTION (minefield, row, col);
                }
                turn++;
            }

            if (command == 5) {
                mode = 1;
                printf ("Gameplay mode activated\n");
            }

            if (command == 6) {
                mode = 0;
                printf ("Debug mode activated\n");
            }

            if (WIN_CHECK_FUNCTION(minefield) == 1) {
            printf("Game Won!\n");
            win = 1;
            }

            if (mode == 1) {
                if (win == -1) {
                    printf ("xx\n/\\\n    00 01 02 03 04 05 06 07\n   -------------------------\n");
                }
                else {
                    printf ("..\n\\/\n    00 01 02 03 04 05 06 07\n   -------------------------\n");
                }
                GAMEPLAY_MODE_FUNCTION(minefield, win);
            }
            if (mode == 0) {
                print_debug_minefield(minefield);
            }
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

// Functions
void DETECT_ROW_FUNCTION(int minefield[SIZE][SIZE]) {
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

void DETECT_COL_FUNCTION(int minefield[SIZE][SIZE]) {
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

int DETECT_SQUARE_FUNCTION(int minefield[SIZE][SIZE], int row, int col, int size, int mine_count) {
    int row_size, col_size;

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
    return mine_count;
}

void REVEAL_SQUARE_FUNCTION (int minefield[SIZE][SIZE], int row, int col) {
    int row_size, col_size;
    int size = 3;

    int mine_count = 0;
    
    
    mine_count = DETECT_SQUARE_FUNCTION(minefield, row, col, size, mine_count);


    if (mine_count > 0) {
        minefield[row][col] = VISIBLE_SAFE;
    }
    else {
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

        int i = 0;
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
}

int WIN_CHECK_FUNCTION (int minefield[SIZE][SIZE]) {
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
    return win_check;
}

void GAMEPLAY_MODE_FUNCTION (int minefield[SIZE][SIZE], int win) {
    int m = 0;

    while (m < SIZE) {
        printf("%02d |", m);

        int n = 0;
        while (n < SIZE){
            if (minefield[m][n] == HIDDEN_MINE && win == -1){
                printf("()");
            }

            else if (minefield[m][n] == HIDDEN_SAFE || minefield[m][n] == HIDDEN_MINE) {
                printf ("##");
            }

            else if (minefield[m][n] == VISIBLE_SAFE) {
                int mine_count = 0;
                int size = 3;
                mine_count = DETECT_SQUARE_FUNCTION(minefield, m, n, size, mine_count);

                if (mine_count == 0) {
                    printf("  ");
                }

                else {
                    printf("%02d", mine_count);
                }

            }
            n++;

            if (m != SIZE) {
                printf(" ");
            }
        }
        m++;
        printf("| \n");
    }
    printf ("   -------------------------\n");

}
void REVEAL_RADIAL_FUNCTION (int minefield[SIZE][SIZE], int row, int col) {
    int mine_count = 0;
    int size = 3;
    minefield[row][col] = VISIBLE_SAFE;
    int i = 0;
    while (i < SIZE) {    
        if (row + i == 7 || col + i == 7) {
            minefield[row + i][col + i] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row + i, col + i, size, mine_count) > 0) {
            minefield[row + i][col + i] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row + i, col + i, size, mine_count) == 0) {
            minefield[row + i][col + i] = VISIBLE_SAFE;
            i++;
        }
    }
    i = 0;
    while (i < SIZE) {    
        if (row + i == 7) {
            minefield[row + i][col] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row + i, col, size, mine_count) > 0) {
            minefield[row + i][col] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row + i, col, size, mine_count) == 0) {
            minefield[row + i][col] = VISIBLE_SAFE;
            i++;
        }
    }
    i = 0;
    while (i < SIZE) {    
        if (col + i == 7) {
            minefield[row][col + i] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row, col + i, size, mine_count) > 0) {
            minefield[row][col + i] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row, col + i, size, mine_count) == 0) {
            minefield[row][col + i] = VISIBLE_SAFE;
            i++;
        }
    }
    i = 0;
    while (i < SIZE) {    
        if (row - i == 0 || col + i == 7) {
            minefield[row - i][col + i] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row - i, col + i, size, mine_count) > 0) {
            minefield[row - i][col + i] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row - i, col + i, size, mine_count) == 0) {
            minefield[row - i][col + i] = VISIBLE_SAFE;
            i++;
        }
    }    
    i = 0;
    while (i < SIZE) {    
        if (row - i == 0) {
            minefield[row - i][col] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row - i, col, size, mine_count) > 0) {
            minefield[row - i][col] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row - i, col, size, mine_count) == 0) {
            minefield[row - i][col] = VISIBLE_SAFE;
            i++;
        }
    }
    i = 0;
    while (i < SIZE) {    
        if (row + i == 7 || col - i == 0) {
            minefield[row + i][col - i] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row + i, col - i, size, mine_count) > 0) {
            minefield[row + i][col - i] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row + i, col - i, size, mine_count) == 0) {
            minefield[row + i][col - i] = VISIBLE_SAFE;
            i++;
        }
    }
    i = 0;
    while (i < SIZE) {    
        if (col - i == 0) {
            minefield[row][col - i] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row, col - i , size, mine_count) > 0) {
            minefield[row][col - i] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row, col - i, size, mine_count) == 0) {
            minefield[row][col - i] = VISIBLE_SAFE;
            i++;
        }
    }
    i = 0;
    while (i < SIZE) {    
        if (row - i == 0 || col - i == 0) {
            minefield[row - i][col - i] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row - i, col - i, size, mine_count) > 0) {
            minefield[row - i][col - i] = VISIBLE_SAFE;
            i = SIZE;
        }
        else if (DETECT_SQUARE_FUNCTION(minefield, row - i, col - i, size, mine_count) == 0) {
            minefield[row - i][col - i] = VISIBLE_SAFE;
            i++;
        }
    }
}