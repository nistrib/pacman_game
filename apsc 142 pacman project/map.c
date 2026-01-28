// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include <stdlib.h>
#include <stdio.h>
#include "defines.h"
#include "colours.h"
#include "map.h"
#include "ghost.h"

extern char *map, *dot_map;
extern int width, height;


// Counting the number of ghosts (this part helps ghosts not to collide on each other), another part is implemented in move_actor
int ghost_num = NUM_GHOSTS;
//function to check for two ghosts in separate map coordinates
int find_ghosts(void) {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(map[i * width + j] == GHOST) {
                // If a second ghost is found, return 0
                if(--ghost_num <= 0) {
                    return 0;
                }
            }
        }
    }
    // Return 1 only if exactly one ghost was found.
    return (ghost_num == 1) ? 1 : 0;
}



// MOVE ACTOR
int move_actor(int *y, int *x, char direction, int eat_dots) {
    int current_y = *y;
    int current_x = *x;
    int new_y = current_y;
    int new_x = current_x;

    // Calculate new pacman position based on direction
    switch (direction) {
        case UP:
            new_y -= 1;
            break;
        case DOWN:
            new_y += 1;
            break;
        case LEFT:
            new_x -= 1;
            break;
        case RIGHT:
            new_x += 1;
            break;
        default:
            return MOVED_INVALID_DIRECTION;
    }

    // Check for map boundaries
    if (new_y < 0 || new_y >= height || new_x < 0 || new_x >= width) {
        return MOVED_INVALID_DIRECTION;
    }

    if (is_wall(new_y, new_x) == YES_WALL) {
        return MOVED_WALL;
    }

    // Check for ghost collision (the first part of the code is applied in find_ghosts function)
    if (map[new_y * width + new_x] == GHOST) {
        return MOVED_GHOST_COLLISION;
    }

    // Perform the move
    char actor_move = map[(current_y * width) + current_x];
    map[(new_y * width) + new_x] = actor_move;

    // Clear the old position
    if (actor_move == GHOST && (dot_map[(current_y * width) + current_x] == DOT)) {
        // If there was a dot underneath the ghost, put the dot back
        map[(current_y * width) + current_x] = DOT;
    } else {
        // Otherwise, make it empty
        map[(current_y * width) + current_x] = EMPTY;
    }

    // If eating dots is enabled, handle that separately
    if (eat_dots == EAT_DOTS && actor_move != GHOST) {
        // Pacman eats the dot and the position should be empty
        map[(current_y) * width + current_x] = EMPTY;
        dot_map[(current_y) * width + current_x] = EMPTY;
    } else if (eat_dots == REPLACE_DOTS && actor_move != GHOST) {
        // Pacman moves away, replace the dot if it was there
        map[(current_y) * width + current_x] = dot_map[(current_y) * width + current_x];
    }

    // Update actor's position
    *y = new_y;
    *x = new_x;

    return MOVED_OKAY;
}


// CHECK WALL PART
    int is_wall(int y, int x) {
//include null statement
        if (y >= height || x >= width){
            return 0;
        }

        else if (map[(y*width) + x] == WALL) {
            return YES_WALL;
        }
        else{
            return NOT_WALL;
        }
    }



//LOAD MAP
char *load_map(char *filename, int *map_height, int *map_width) {

    // Open FILE
    FILE* file = fopen(filename, "r");
    if (!file) { // Return NULL if file cannot be opened
        printf("Error, could not open file %s\n", filename);
        return NULL;
    }

    // Determining the dimension of the map
    int dimension;
    *map_width = 0;
    *map_height = 1; // Initialize to 1 as there is no newline at the end of the file
    int temp_width = 0;

    // While loop to determine the width and height of the map
    while ((dimension = fgetc(file)) != EOF) {
        if (dimension == '\n') {
            (*map_height)++;
            if (*map_width < temp_width) {
                *map_width = temp_width;
            }
            temp_width = 0;
        } else if (dimension != ' ') {
            temp_width++;
        }
    }


    // Adjusting map_width for the last line if it doesn't end with a newline character
    if (temp_width > 0) {
        if (*map_width < temp_width) {
            *map_width = temp_width;
        }
    }

    // Allocating memory for the map based on the dimensions
    map = malloc((*map_height) * (*map_width) * sizeof(char));
    if (map == NULL) {
        printf("Memory could not be allocated.\n");
        fclose(file);
        return NULL;
    }

    // Reset file pointer to start to read the map again
    rewind(file);

    // Loading the map into the allocated memory
    int row = 0, column = 0;
    while ((dimension = fgetc(file)) != EOF) {
        if (dimension == '\n') {
            row++;
            column = 0;
        } else if (dimension != ' ') {
            int index = row * (*map_width) + column;
            map[index] = (char)dimension;
            column++;
        }
    }

    // Closing the file and returning the map
    fclose(file);
    return map;
}



// PRINT MAP
void print_map(char*map_array, int mapHeight, int mapWidth) {

    // Printing top wall
    change_text_colour(BLUE);
    for(int i = 0; i < mapWidth + 2; i++){
        printf("W ");
    }
    printf("\n");

    // Printing walls around the map and making color
    for(int i = 0; i < mapHeight; i++){
        change_text_colour(BLUE);
        printf("W "); // Print left wall
        for(int j = 0; j < mapWidth; j++){
            char map_char = map_array[i * mapWidth + j];
            switch(map_char) {
                case PACMAN:
                    change_text_colour(YELLOW);
                    break;
                case GHOST:
                    change_text_colour(PINK);
                    break;
                case DOT:
                    change_text_colour(WHITE);
                    break;
                case WALL:
                    change_text_colour(BLUE);
                    break;
                default:
                    change_text_colour(WHITE); // Default color for everything else
            }
            printf("%c ", map_char);
            if(map_char != WALL) { // Reset color after each character if it's not a wall to prevent color bleed
                change_text_colour(WHITE);
            }
        }
        change_text_colour(BLUE);
        printf("W\n");
    }

    // Bottom wall
    change_text_colour(BLUE);
    for(int i = 0; i < mapWidth + 2; i++){
        printf("W ");
    }
    printf("\n");
}

// LOAD DOT MAP
char *load_dot_map(char *filename, int *map_height, int *map_width) {
    FILE *file = fopen(filename, "r");
    if (!file) { // Return NULL if file cannot be opened
        printf("Error, could not open file %s\n", filename);
        return NULL;
    }

    // Determining the dimension of the map
    int dimension;
    *map_width = 0;
    *map_height = 1; // Initialize to 1 as there is no newline at the end of the file
    int temp_width = 0;

    // While loop to determine the width and height of the map
    while ((dimension = fgetc(file)) != EOF) {
        if (dimension == '\n') {
            (*map_height)++;
            if (*map_width < temp_width) {
                *map_width = temp_width;
            }
            temp_width = 0;
        } else if (dimension != ' ') {
            temp_width++;
        }
    }

    // Adjusting map_width for the last line if it doesn't end with a newline character
    if (temp_width > 0) {
        if (*map_width < temp_width) {
            *map_width = temp_width;
        }
    }

    // Allocating memory for the dot map based on the dimensions
    dot_map = malloc((*map_height) * (*map_width) * sizeof(char));
    if (dot_map == NULL) {
        printf("Memory could not be allocated.\n");
        fclose(file);
        return NULL;
    }

    // Reset file pointer to start to read the map again
    rewind(file);

    // Loading the dot map into the allocated memory
    int row = 0, column = 0;
    while ((dimension = fgetc(file)) != EOF) {
        if (dimension == '\n') {
            row++;
            column = 0;
        } else if (dimension != ' ') {
            int index = row * (*map_width) + column;
            dot_map[index] = (dimension == DOT) ? DOT : EMPTY;
            column++;
        }
    }

    // Closing the file and returning the dot map
    fclose(file);
    return dot_map;
}
