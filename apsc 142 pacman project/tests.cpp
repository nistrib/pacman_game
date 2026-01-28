// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <stdio.h>

// make sure not to modify anything in this extern block
extern "C"{
#include "defines.h"
#include "map.h"
#include "ghost.h"
#include "game.h"
char *map = NULL, *dot_map = NULL;
int width, height;
}

/**
 * This file is where you should put your tests for your code.
 * Your code must have tests that execute at least 85% of the code in
 * required functions for you to get full marks for the project.
 * Make sure to check out the course videos on automated testing for
 * more information about how to write tests.
 */

void setup(void) {
    map = load_map("map.txt", &height, &width);
}

void teardown() {
    free(map);
    map = NULL;
}

/* tests for map.c */
TEST_SUITE_BEGIN("Map tests");

// tests for load_map
TEST_CASE("A test for load_map") {
    setup();

    char* filename = "map.txt";
    char* result = load_map(filename, &height, &width);
    CHECK(result != NULL);

    teardown();
}

TEST_CASE("load_map returns NULL for a non-existent map file") {
    setup();
    char* filename = "non_existent_map.txt";
    char* result = load_map(filename, &height, &width);
    CHECK(result == NULL);
    teardown();
}

TEST_CASE("load_map returns NULL for an improperly formatted map file") {
    setup();
    char* filename = "improperly_formatted_map.txt"; // Improperly formatted map file
    char* result = load_map(filename, &height, &width);
    CHECK(result == NULL);
    teardown();
}

TEST_CASE("load_map handles empty map file") {
    setup();
    char* filename = "empty_map.txt"; // Empty map file
    char* result = load_map(filename, &height, &width);
    CHECK(result == NULL);
    teardown();
}
// tests for is_wall
TEST_CASE("is_wall identifies walls correctly") {
    setup();

    // Test when the coordinates point to a wall
    CHECK(is_wall(0, 4) == YES_WALL);

    // Test when the coordinates point to an open space
    CHECK(is_wall(1, 0) == NOT_WALL);

    // Test when the coordinates point to a ghost
    map[2 * width + 2] = GHOST;
    CHECK(is_wall(2, 2) == NOT_WALL);

    // Test when the coordinates point to Pacman
    map[3 * width + 3] = PACMAN;
    CHECK(is_wall(3, 3) == NOT_WALL);

    // Test when the coordinates are out of the map boundaries
    CHECK(is_wall(1, 0) == NOT_WALL);
    CHECK(is_wall(0, -1) == YES_WALL);
    CHECK(is_wall(height, 0) == YES_WALL);
    CHECK(is_wall(0, width) == YES_WALL);
    CHECK(is_wall(1000, 1000) == YES_WALL);

    teardown();
}

TEST_SUITE_END();


TEST_SUITE_BEGIN("sees_pacman tests");

TEST_CASE("sees_pacman returns EATING_PACMAN when Pacman and the ghost occupy the same spot") {
    setup();

    int pacman_y = 2, pacman_x = 2;
    int ghost_y = 2, ghost_x = 2;

    CHECK(sees_pacman(pacman_y, pacman_x, ghost_y, ghost_x) == EATING_PACMAN);

    teardown();
}

TEST_CASE("sees_pacman returns the correct direction when Pacman is in the same column as the ghost") {
    setup();

    int pacman_y = 3, pacman_x = 2;
    int ghost_y = 1, ghost_x = 2;

    CHECK(sees_pacman(pacman_y, pacman_x, ghost_y, ghost_x) == DOWN);

    pacman_y = 1;
    ghost_y = 3;

    CHECK(sees_pacman(pacman_y, pacman_x, ghost_y, ghost_x) == UP);

    teardown();
}

TEST_CASE("sees_pacman returns the correct direction when Pacman is in the same row as the ghost") {
    setup();

    int pacman_y = 2, pacman_x = 3;
    int ghost_y = 2, ghost_x = 1;

    CHECK(sees_pacman(pacman_y, pacman_x, ghost_y, ghost_x) == RIGHT);

    pacman_x = 1;
    ghost_x = 3;

    CHECK(sees_pacman(pacman_y, pacman_x, ghost_y, ghost_x) == LEFT);

    teardown();
}

TEST_CASE("sees_pacman returns SEES_NOTHING when Pacman is not visible to the ghost") {
    setup();

    int pacman_y = 1, pacman_x = 1;
    int ghost_y = 3, ghost_x = 3;

    CHECK(sees_pacman(pacman_y, pacman_x, ghost_y, ghost_x) == SEES_NOTHING);

    teardown();
}

TEST_SUITE_END();


TEST_SUITE_BEGIN("Game tests");

TEST_CASE("check_win returns YOU_WIN when only one dot remains and it's at Pacman's position") {
    setup();

    // Setting up the map so that only one dot remains and it's at pacman position
    map[2 * width + 2] = PACMAN;
    dot_map[2 * width + 2] = DOT;

    int pacman_y = 2, pacman_x = 2;
    int ghosts_y[NUM_GHOSTS] = {3, 3};
    int ghosts_x[NUM_GHOSTS] = {3, 4};

    CHECK(check_win(pacman_y, pacman_x, ghosts_y, ghosts_x) == YOU_WIN);

    teardown();
}

TEST_CASE("check_win returns KEEP_GOING when more than one dot remains") {
    setup();

    // Set up the map so that more than one dot remains
    // You may need to adjust this setup to match your specific map layout
    map[2 * width + 2] = PACMAN;
    dot_map[2 * width + 2] = DOT;
    dot_map[3 * width + 3] = DOT;

    int pacman_y = 2, pacman_x = 2;
    int ghosts_y[NUM_GHOSTS] = {3, 3};
    int ghosts_x[NUM_GHOSTS] = {3, 4};

    CHECK(check_win(pacman_y, pacman_x, ghosts_y, ghosts_x) == KEEP_GOING);

    teardown();
}

TEST_CASE("check_loss returns YOU_LOSE when Pacman is at the same position as any ghost") {
    setup();

    int pacman_y = 2, pacman_x = 2;
    int ghosts_y[NUM_GHOSTS] = {2, 3};
    int ghosts_x[NUM_GHOSTS] = {2, 3};

    CHECK(check_loss(pacman_y, pacman_x, ghosts_y, ghosts_x) == YOU_LOSE);

    teardown();
}

TEST_CASE("check_loss returns KEEP_GOING when Pacman is not at the same position as any ghost") {
    setup();

    int pacman_y = 2, pacman_x = 2;
    int ghosts_y[NUM_GHOSTS] = {3, 3};
    int ghosts_x[NUM_GHOSTS] = {3, 4};

    CHECK(check_loss(pacman_y, pacman_x, ghosts_y, ghosts_x) == KEEP_GOING);

    teardown();
}

TEST_SUITE_END();