/**
 * @file replay.c
 * @author Faris Soliman
 * This function lets a user replay a game they played.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "game.h"
#include <unistd.h>
#include "error-codes.h"
#include "io.h"

/**
 * The main function.
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @return The exit code.
*/
int main(int argc, char** argv) {
    // Check the number of arguments.
    if (argc != 2) {
        fprintf(stdout, "usage: %s <saved-match.gmk>\n", argv[0]);
        return ARGUMENT_ERR;
    }

    // Load the saved game
    game* savedGame = game_import(argv[1]);

    if (savedGame == NULL) {
        printf("Failed to load the saved game.\n");
        return FILE_INPUT_ERR;
    }

    game* replayGame = game_create(savedGame->board->size, savedGame->type);
    replayGame->state = savedGame->state;
    replayGame->winner = savedGame->winner;

    // Replay the game
    game_replay(savedGame);

    // Delete the game
    game_delete(replayGame);

    return SUCCESS;
}