// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include "defines.h"
#include "game.h"

extern char *map, *dot_map;
extern int height;
extern int width;


// CHECK WIN
int check_win(int pacman_y, int pacman_x, int ghosts_y[NUM_GHOSTS], int ghosts_x[NUM_GHOSTS]) {
    int dotCounter = 0;
    int lastDotIndex = -1; // Initialized to an invalid index

    for (int i = 0; i < width * height; i++) {
        if (dot_map[i] == DOT) {
            dotCounter++;
            lastDotIndex = i; // The last dot's position
            if (dotCounter > 1) {
                return KEEP_GOING; // Early exit if more than one dot remains
            }
        }
    }

    // If exactly one dot remains, check if it's at pacman's  position
    if (dotCounter == 1 && map[lastDotIndex] == PACMAN) {
        return YOU_WIN;
    }

    return KEEP_GOING;
}

// CHECK LOSS
int check_loss(int pacman_y, int pacman_x, int ghosts_y[NUM_GHOSTS], int ghosts_x[NUM_GHOSTS]) {
    for (int i = 0; i < NUM_GHOSTS; i++) {
        if (pacman_y == ghosts_y[i] && pacman_x == ghosts_x[i]) {
            return YOU_LOSE;
        }
    }
    return KEEP_GOING;
}
