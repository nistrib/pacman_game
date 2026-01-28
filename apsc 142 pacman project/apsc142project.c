// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

// don't forget to update your project configuration to select "Emulate terminal in the output console"

// Make sure to include all relevant libraries
#include <stdio.h>
#include <stdlib.h>

// colours.h contains functions to change text colour and read single characters without requiring an enter
#include "colours.h"
// defines.h contains useful definitions to keep your code readable
#include "defines.h"
// map.h, game.h, and ghost.h contain prototypes of functions you must implement
#include "map.h"
//#include "map.c"
#include "game.h"
#include "ghost.h"

// These global variables must be used to store map information.
// Almost every function needs these variables, so keeping them as globals helps keep things organized.
// map is a pointer to a dynamically allocated map for displaying to the user
// dot_map is a pointer to a dynamically allocated map for keeping track of what dots are left
char *map = NULL, *dot_map = NULL;
// width and height store the width and height of map, NOT counting outer walls
int width, height;



/**
 * Main entry point into your program.
 * Make sure that main returns appropriate status codes depending on what
 * happens.  The codes you must use are:
 *   NO_ERROR when no error occurs
 *   ERR_NO_MAP when no map file is found
 *   ERR_NO_PACMAN when no pacman is found on the map
 *   ERR_NO_GHOSTS when fewer than 2 ghosts are found on the map
 *
 * Make sure that any allocated memory is freed before returning.
 * @return a status code
 */
int main(void) {
    setbuf(stdout, NULL);

    // Load the map file into the map and dot_map arrays
    char* filename = MAP_NAME;
    map = load_map(filename, &height, &width);
    dot_map = load_dot_map(filename, &height, &width);

    // Position of pacman and ghosts
    int pacman_x = 4, pacman_y = 4;
    int ghosts_x[NUM_GHOSTS] = {0, 8}, ghosts_y[NUM_GHOSTS] = {0, 8};

    while (1) {
        print_map(map, height, width);

        // Check if the user has won or lost
        if (check_win(pacman_y, pacman_x, ghosts_y, ghosts_x) == YOU_WIN) {
            printf("Congratulations! You win!\n");
            break;
        }
        if (check_loss(pacman_y, pacman_x, ghosts_y, ghosts_x) == YOU_LOSE) {
            printf("Sorry, you lose.\n");
            break;
        }

        // Get the direction from the user and move pacman
        char direction = getch();
        move_actor(&pacman_y, &pacman_x, direction, EAT_DOTS);

        // Move the ghosts
        for (int i = 0; i < NUM_GHOSTS; i++) {
            int randomNumber = rand() % 4;
            char directionGhost = (randomNumber == 0) ? UP :
                                  (randomNumber == 1) ? RIGHT :
                                  (randomNumber == 2) ? DOWN : LEFT;
            char seesPacman = sees_pacman(pacman_y, pacman_x, ghosts_y[i], ghosts_x[i]);
            if (seesPacman != SEES_NOTHING) {
                move_actor(&ghosts_y[i], &ghosts_x[i], seesPacman, REPLACE_DOTS);
            } else {
                move_actor(&ghosts_y[i], &ghosts_x[i], directionGhost, REPLACE_DOTS);
            }
        }
    }

    // Free the memory allocated for the map and dot_map
    free(map);
    free(dot_map);

    return NO_ERROR;
}

