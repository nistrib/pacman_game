// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include "defines.h"
#include "ghost.h"
#include "map.h"

// Checks if a ghost can see Pacman, and in what direction they must look to see him.
char sees_pacman(int pacman_y, int pacman_x, int ghost_y, int ghost_x) {
    // Check if Pacman and the ghost occupy the same spot.
    if (pacman_y == ghost_y && pacman_x == ghost_x) {
        return EATING_PACMAN;
    }

    // Check if Pacman is in the same column as the ghost.
    if (pacman_x == ghost_x) {
        int step = (pacman_y > ghost_y) ? 1 : -1; // Determine step direction based on  position.
        for (int i = ghost_y + step; i != pacman_y; i += step) {
            if (is_wall(i, ghost_x) == YES_WALL) {
                return SEES_NOTHING;
            }
        }
        return (step == 1) ? DOWN : UP; // Determine direction based on step.
    }

    // Checking Pacman is in the same row as the ghost.
    if (pacman_y == ghost_y) {
        int step = (pacman_x > ghost_x) ? 1 : -1;
        for (int i = ghost_x + step; i != pacman_x; i += step) {
            if (is_wall(ghost_y, i) == YES_WALL) {
                return SEES_NOTHING;
            }
        }
        return (step == 1) ? RIGHT : LEFT; // Determine direction based on step.
    }

    // If none of the conditions are met, Pacman is not visible.
    return SEES_NOTHING;
}
