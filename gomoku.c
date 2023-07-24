/**
 * @file gomoku.c
 * @author Faris Soliman
 * This file creates and manages a gomoku freestyle game
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "io.h"
#include "error-codes.h"

/**
 * The main function.
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @return The exit code.
 */
int main(int argc, char** argv) 
{
    // Check the number of arguments.
    if (argc < 1 || argc > 7) 
    {
        fprintf(stderr, "usage: %s [-o /path/to/file] [-r /path/to/file] [-b size]\n", argv[0]);
        return ARGUMENT_ERR;
    }

    // Check the arguments.
    int size = 15;
    char* saveFile = NULL;
    char* loadFile = NULL;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            for (int j = 0; j < argc; j++) {
                if (strcmp(argv[j], "-b") == 0) {
                    fprintf(stdout, "usage: %s [-r <unfinished-match.gmk>] [-o <saved-match.gmk>] [-b <15|17|19>] \n", argv[0]);
                    printf("       -r and -b conflicts with each other \n");
                    return ARGUMENT_ERR;
                }
            }
        }
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "usage: %s [-o /path/to/file] [-r /path/to/file] [-b size]\n", argv[0]);
                return ARGUMENT_ERR;
            }
            saveFile = argv[i + 1];
            i++;
        } 
        else if (strcmp(argv[i], "-r") == 0) {
            if (i + 1 >= argc) 
            {
                fprintf(stderr, "usage: %s [-o /path/to/file] [-r /path/to/file] [-b size]\n", argv[0]);
                return ARGUMENT_ERR;
            }
            loadFile = argv[i + 1];
            i++;
        } 
        else if (strcmp(argv[i], "-b") == 0) {
            if (i + 1 >= argc) 
            {
                fprintf(stderr, "usage: %s [-o /path/to/file] [-r /path/to/file] [-b size]\n", argv[0]);
                return ARGUMENT_ERR;
            }
            size = atoi(argv[i + 1]);
            if (size == 0) {
                fprintf(stdout, "usage: %s [-r <unfinished-match.gmk>] [-o <saved-match.gmk>] [-b <15|17|19>]\n", argv[0]);
                printf("       -r and -b conflicts with each other\n");
                return ARGUMENT_ERR;
            } else if (size != 15 && size != 17 && size != 19) 
            {
                return BOARD_SIZE_ERR;
            }
            i++;
        } 
        else {
            fprintf(stderr, "usage: %s [-o /path/to/file] [-r /path/to/file] [-b size]\n", argv[0]);
            return ARGUMENT_ERR;
        }
    }

    // Create the game.
    game* game = NULL;

    // Load the game if necessary.
    if (loadFile != NULL) {
        game = game_import(loadFile);
        if (game->state != GAME_STATE_FINISHED) {
            board_print(game->board, true);
            game_resume(game);
        } else {
            return RESUME_ERR;
        }
    }
    else {
        game = game_create(size, GAME_FREESTYLE);
        board_print(game->board, true);
        game_loop(game);
    }

    // Save the game if necessary.
    if (saveFile != NULL) {
        game_export(game, saveFile);
    }

    if (GAME_STATE_FINISHED == game->state) {
        if (board_is_full(game->board)) {
            printf("Game concluded, the board is full, draw.\n");
        }
    }

    // Delete the game.
    game_delete(game);

    return SUCCESS;
}