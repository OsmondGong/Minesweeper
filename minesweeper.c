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
void detect_row(int minefield[SIZE][SIZE]);
void detect_col(int minefield[SIZE][SIZE]);
int detect_square(int minefield[SIZE][SIZE], int row, int col, int size);
int mine(int minefield[SIZE][SIZE], int row, int col, int r_size, int c_size);
void reveal_square(int minefield[SIZE][SIZE], int row, int col);
int win_check(int minefield[SIZE][SIZE]);
void  gameplay(int minefield[SIZE][SIZE], int win);
void reveal_radial(int minefield[SIZE][SIZE], int row, int col);
void prints_mine_arround_square(int minefield[SIZE][SIZE], int i, int j);
void turn_one_radial(int minefield[SIZE][SIZE], int row, int col);

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

        // Places mines only if coordinate is on the minefield
        int ycoord_bool, xcoord_bool;
        ycoord_bool = (-1 < ycoord) && (ycoord < SIZE);
        xcoord_bool = (-1 < xcoord) && (xcoord < SIZE);
        if (ycoord_bool && xcoord_bool) {
            minefield[ycoord][xcoord] = 2;
        }
        coord++;
    }

    printf("Game Started\n");
    print_debug_minefield(minefield);

    int mode = 0; // Used to change modes
    int win = 0; // Used to loop game until user wins/loses
    int hint = 0; // Used to limit amount of hints
    int turn = 1; // Used to ensure user does not lose on the first turn
    
    // TODO: Scan in commands to play the game until the game ends.
    while (win == 0) {
        int command = 0;
        scanf("%d", &command);
        // EOF (Ctrl+D) and any input that is not 1 to 7 will end the game when entered
        if (command > 0) {
            // Ensures only 3 hints will be given
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
                detect_row(minefield);
                hint++;
            }

            else if (command == DETECT_COL) {
                detect_col(minefield);
                hint++;
            }

            else if (command == DETECT_SQUARE) {
                int row, col, size;

                scanf("%d", &row);
                scanf("%d", &col);
                scanf("%d", &size);

                int num = detect_square(minefield, row, col, size);
                printf("There are %d mine(s) in the square centered", num);
                printf(" at row %d, column %d of size %d\n", row, col, size);
                hint++;
            }

            else if (command == REVEAL_SQUARE) {
                int row, col;

                scanf("%d", &row);
                scanf("%d", &col);
                
                // If player selects mine, the game ends
                if (minefield[row][col] == HIDDEN_MINE) {
                    // If player selects mine on first turn, all mines are shift down a row
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
                        reveal_square(minefield, row, col);
                    }
                    else {
                        win = -1;
                        printf("Game over\n");
                    }
                }
                else {
                    reveal_square(minefield, row, col);
                }
                turn++;
            }

            else if (command == REVEAL_RADIAL) {
                int row, col;

                scanf("%d", &row);
                scanf("%d", &col);

                // If player selects mine, the game ends
                if (minefield[row][col] == HIDDEN_MINE) {
                    // If player selects mine on first turn, all mines are shift down a row
                    if (turn == 1) {
                        turn_one_radial(minefield, row, col);
                    }
                    else {
                        win = -1;
                        printf("Game over\n");
                    }
                }
                else {
                    reveal_radial(minefield, row, col);
                }
                turn++;
            }

            if (command == GAMEPLAY_MODE) {
                mode = 1;
                printf(" gameplay mode activated\n");
            }

            if (command == DEBUG_MODE) {
                mode = 0;
                printf("Debug mode activated\n");
            }

            if (win_check(minefield) == 1) {
                printf("Game Won!\n");
                win = 1;
            }

            if (mode == 1) {
                // Prints a frown when game is lost and a smile when game isn't lost
                if (win == -1) {
                    printf("xx\n/\\\n    00 01 02 03 04 05 06 07\n");
                    printf("   -------------------------\n");
                }
                else {
                    printf("..\n\\/\n    00 01 02 03 04 05 06 07\n");
                    printf("   -------------------------\n");
                }
                gameplay(minefield, win);
            }
            if (mode == 0) {
                print_debug_minefield(minefield);
            }
        }
        else {
            win = 1;
        }

    }
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
// Scans row input and outputs the amount of mines in that row
void detect_row(int minefield[SIZE][SIZE]) {
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

// Scans column input and outputs the amount of mines in that column
void detect_col(int minefield[SIZE][SIZE]) {
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

// Uses row, col and size input and outputs the amount of mines in the square 
// with centre at input row and column with size of the input size
int detect_square(int minefield[SIZE][SIZE], int row, int col, int size) {
    // the size of the square is separated into two; the amount of rows and 
    // the amount of columns of the square
    int r_size, c_size;

    // If input size is even, the size is reduced by 1 as size must be odd
    if (size % 2 == 0 && size > 0) {
        size--;
    }
    
    // col_edge is left-most column of square
    int col_edge = 0;
    // If inputted column is at the edge of the minefield, the portion of the 
    // square that is outside of the minefield is omitted
    if (col == 0) {
        c_size = size / 2 + 1;
        col_edge = 1;
    }
    else if (col == 7) {
        c_size = size / 2 + 1;
        col = col - size / 2;
    }
    else {
        col = col - size / 2;
        c_size = size;
    }
    // row_edge is top-most row of square
    int row_edge = 0;
    // If inputted row is at the edge of the minefield, the portion of the 
    // square that is outside of the minefield is omitted
    if (row == 0) {
        r_size = size / 2 + 1;
        row_edge = 1;
    }                
    else if (row == 7) {
        r_size = size / 2 + 1;
        row = row - size / 2;
    }
    else {
        row = row - size / 2;
        r_size = size;
    }
    int mine_count = 0;
    // Scans the square and counts the amount of mines
    if (size >= 0) {
        mine_count = mine(minefield, row, col, r_size, c_size);
    }    
    // reverts the row and column values back into its inputted value
    row = row - r_size;
    if (row_edge == 0) {
        row = row + size / 2;
    }
    if (col_edge == 0) {
        col = col + size / 2;
    }
    return mine_count;
}
int mine(int minefield[SIZE][SIZE], int row, int col, int r_size, int c_size) {
    int mine_count = 0;
    int i = 0;
    while (i < r_size) {
        int j = 0;
        while (j < c_size ) {
            if (minefield[row][col] == HIDDEN_MINE) {
                mine_count++;
            }
            j++;
            col++;
        }
        col = col - c_size;
        i++;
        row++;
    }
    return mine_count;
}
// Uses row and col input and reveals a 3x3 square with centre at row and col
// input or the block at inputted row and column
void reveal_square(int minefield[SIZE][SIZE], int row, int col) {
    int size = 3;

    int mine_count = 0;
    
    mine_count = detect_square(minefield, row, col, size);

    // prints selected row and column if there are mines surrounding it
    if (mine_count > 0) {
        minefield[row][col] = VISIBLE_SAFE;
    }
    // prints entire square if there are no mines surrounding it
    else {
        // separates size into column and row sizes for when rows and columns 
        // are on the edge
        int r_size, c_size;
        int col_edge = 0;
        if (col == 0) {
            c_size = size / 2 + 1;
            col_edge = 1;
        }
        
        else if (col == 7) {
            c_size = size / 2 + 1;
            col = col - size / 2;
        }

        else {
            col = col - size / 2;
            c_size = size;
        }

        int row_edge = 0;

        if (row == 0) {
            r_size = size / 2 + 1;
            row_edge = 1;
        }        

        else if (row == 7) {
            r_size = size / 2 + 1;
            row = row - size / 2;
        }

        else {
            row = row - size / 2;
            r_size = size;
        }

        int i = 0;
        while (i < r_size) {
            int j = 0;
            while (j < c_size) {
                minefield[row][col] = VISIBLE_SAFE;
                j++;
                col++;
            }
            col = col - c_size;
            i++;
            row++;
        }

        row = row - r_size;

        if (row_edge == 0) {
            row = row + size / 2;
        }

        if (col_edge == 0) {
            col = col + size / 2;
        }
    }
}

// Scans entire minefield to count amount of safe hidden block
int win_check(int minefield[SIZE][SIZE]) {
    int win_check = 1;
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            if (minefield[i][j] == HIDDEN_SAFE) {
                win_check = 0;
            }
            j++;
        }
        i++;
    }
    return win_check;
}

// Gameplay mode
void gameplay(int minefield[SIZE][SIZE], int win) {
    // Replaces minefield with symbols, hiding mines and safe squares
    int i = 0;
    while (i < SIZE) {
        printf("%02d |", i);
        int j = 0;
        while (j < SIZE ) {
            // if game is lost, the mines are revealed
            if (minefield[i][j] == HIDDEN_MINE && win == -1) {
                printf("()");
            }
            // Hidden mines and hidden squares are both displayed as ##
            else if (minefield[i][j] == HIDDEN_SAFE || minefield[i][j] == HIDDEN_MINE) {
                printf("##");
            }
            // Revealed squares show the amount of mines that are surrounding it
            else if (minefield[i][j] == VISIBLE_SAFE) {
                prints_mine_arround_square(minefield, i, j);
            }
            j++;
            // prints a space between the symbols but not after the last symbol of each
            // row 
            if (i != SIZE) {
                printf(" ");
            }
        }
        i++;
        printf("| \n");
    }
    printf("   -------------------------\n");

}
void prints_mine_arround_square(int minefield[SIZE][SIZE], int i, int j) {
    int mine_count = 0;
    int size = 3;
    mine_count = detect_square(minefield, i, j, size);
    
    // if there are no mines surrounding the square, the square shows nothing
    if (mine_count == 0) {
        printf("  ");
    }

    else {
        printf("%02d", mine_count);
    }
}

// Reveals a radial with centre at inputted row and column but does not reveal
// further if there are mines in the surrounding area of the block
void reveal_radial(int minefield[SIZE][SIZE], int row, int col) {
    int size = 3;
    minefield[row][col] = VISIBLE_SAFE;
    int i = 0;
    // Reveals diagonally from the centre towards the bottom right, scanning 
    // if each square has any mines surrounding it. If it does, it stops 
    // revealing.
    while (i < SIZE) {    
        if (row + i == 7 || col + i == 7) {
            minefield[row + i][col + i] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row + i, col + i, size) > 0) {
            minefield[row + i][col + i] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row + i, col + i, size) == 0) {
            minefield[row + i][col + i] = VISIBLE_SAFE;
            i++;
        }
    }
    i = 0;
    while (i < SIZE) {    
        // Reveals downwards from the centre
        if (row + i == 7) {
            minefield[row + i][col] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row + i, col, size) > 0) {
            minefield[row + i][col] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row + i, col, size) == 0) {
            minefield[row + i][col] = VISIBLE_SAFE;
            i++;
        }
    }
    i = 0;
    while (i < SIZE) {    
        // Reveals horizontally from the centre towards the right,
        if (col + i == 7) {
            minefield[row][col + i] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row, col + i, size) > 0) {
            minefield[row][col + i] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row, col + i, size) == 0) {
            minefield[row][col + i] = VISIBLE_SAFE;
            i++;
        }
    }
    i = 0;
    // Reveals diagonally up right from the centre
    while (i < SIZE) {    
        if (row - i == 0 || col + i == 7) {
            minefield[row - i][col + i] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row - i, col + i, size) > 0) {
            minefield[row - i][col + i] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row - i, col + i, size) == 0) {
            minefield[row - i][col + i] = VISIBLE_SAFE;
            i++;
        }
    }    
    i = 0;
    // Reveals vertically up from the centre
    while (i < SIZE) {    
        if (row - i == 0) {
            minefield[row - i][col] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row - i, col, size) > 0) {
            minefield[row - i][col] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row - i, col, size) == 0) {
            minefield[row - i][col] = VISIBLE_SAFE;
            i++;
        }
    }
    i = 0;
    // Reveals diagonally down left from the centre
    while (i < SIZE) {    
        if (row + i == 7 || col - i == 0) {
            minefield[row + i][col - i] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row + i, col - i, size) > 0) {
            minefield[row + i][col - i] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row + i, col - i, size) == 0) {
            minefield[row + i][col - i] = VISIBLE_SAFE;
            i++;
        }
    }
    i = 0;
    // Reveals horizontally from the centre towards the left,
    while (i < SIZE) {    
        if (col - i == 0) {
            minefield[row][col - i] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row, col - i , size) > 0) {
            minefield[row][col - i] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row, col - i, size) == 0) {
            minefield[row][col - i] = VISIBLE_SAFE;
            i++;
        }
    }
    i = 0;
    // Reveals diagonally up left from the centre
    while (i < SIZE) {    
        if (row - i == 0 || col - i == 0) {
            minefield[row - i][col - i] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row - i, col - i, size) > 0) {
            minefield[row - i][col - i] = VISIBLE_SAFE;
            break;
        }
        else if (detect_square(minefield, row - i, col - i, size) == 0) {
            minefield[row - i][col - i] = VISIBLE_SAFE;
            i++;
        }
    }
}
void turn_one_radial(int minefield[SIZE][SIZE], int row, int col) {
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
    reveal_radial(minefield, row, col);
}