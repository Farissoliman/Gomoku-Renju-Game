#ifndef _GAME_H
#define _GAME_H
#include "board.h"
#include <stdbool.h>
#include <stdlib.h>
#define GAME_FREESTYLE 0
#define GAME_RENJU 1
#define GAME_STATE_PLAYING 0
#define GAME_STATE_FORBIDDEN 1
#define GAME_STATE_STOPPED 2
#define GAME_STATE_FINISHED 3

typedef struct {
    unsigned char x;
    unsigned char y;
    unsigned char stone;
} move;

typedef struct {
    board* board;
    unsigned char type;
    unsigned char stone;
    unsigned char state;
    unsigned char winner;
    move* moves;
    size_t moves_count;
    size_t moves_capacity;
} game;

game* game_create(unsigned char board_size, unsigned char game_type);
void game_delete(game* g);
bool game_update(game* g);
void game_loop(game* g);
void game_resume(game* g);
void game_replay(game* g);
bool game_place_stone(game* g, unsigned char x, unsigned char y);
#endif
