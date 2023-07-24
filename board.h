#ifndef _BOARD_H_
#define _BOARD_H_
#include <stdbool.h>
#define EMPTY_INTERSECTION 0
#define BLACK_STONE 1
#define WHITE_STONE 2
#define clear() printf("\033[H\033[J")

typedef struct {
    unsigned char size;
    unsigned char* grid;
} board;

board* board_create(unsigned char size);
void board_delete(board* b);
void board_print(board* b, bool in_place);
unsigned char board_formal_coord(board* b, unsigned char x, unsigned char y, char* formal_coord);
unsigned char board_coord(board* b, const char* formal_coord, unsigned char* x, unsigned char* y);
unsigned char board_get(board* b, unsigned char x, unsigned char y);
void board_set(board* b, unsigned char x, unsigned char y, unsigned char stone);
bool board_is_full(board* b);
#endif
