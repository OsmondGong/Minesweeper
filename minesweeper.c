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