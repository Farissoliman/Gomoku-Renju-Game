/**
 * @file io.c
 * @author Faris Soliman
 * This file contains the implementation of the functions used to import and export games.
 */
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "io.h"
#include "error-codes.h"
#include "board.h"

/**
 * Imports a game from a file.
 * @param path The path to the file.
 * @return The game.
 */
game* game_import(const char* path) {
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        exit(FILE_INPUT_ERR);
    }

    char magic[3];
    fgets(magic, 3, fp);
    if (magic[0] != 'G' || magic[1] != 'A') {
        fclose(fp);
        exit(FILE_INPUT_ERR);
    }

    unsigned int board_size;
    int game_type;
    int game_state;
    int winner;
    fscanf(fp, "%u\n%d\n%d\n%d\n", &board_size, &game_type, &game_state, &winner);

    game* g = game_create(board_size, game_type);
    g->state = GAME_STATE_PLAYING;
    g->winner = winner;

    while (!feof(fp)) {
        char x;
        int y;
        fscanf(fp, "%c%d\n", &x, &y);
        game_place_stone(g, x - 'A', y - 1);
    }

    g->state = game_state;

    fclose(fp);
    return g;
}

/**
 * Exports a game to a file.
 * @param g The game.
 * @param path The path to the file.
 */
void game_export(game* g, const char* path) {
    FILE* fp = fopen(path, "w");
    if (fp == NULL) {
      exit(FILE_OUTPUT_ERR);
    }

    fprintf(fp, "GA\n%u\n%d\n%d\n%d\n", g->board->size, g->type, g->state, g->winner);

    for (int i = 0; i < g->moves_count; i++) {
        fprintf(fp, "%c%d\n", g->moves[i].x + 'A', g->moves[i].y + 1);
    }

    fclose(fp);
}