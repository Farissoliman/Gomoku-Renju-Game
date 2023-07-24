/**
 * @file io.c
 * @author Faris Soliman
 * This file contains the methods used to create, delete, and print boards.
 * It also contains the methods used to convert between formal and board coordinates
 * and adding stones to the board.
 */
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "error-codes.h"

/**
 * Creates a new board.
 * @param size The size of the board.
*/
board* board_create(unsigned char size) {
  board* b = malloc(sizeof(board));
  if (b == NULL) {
    exit(NULL_POINTER_ERR);
  }

  b->size = size;
  b->grid = malloc(sizeof(unsigned char) * size * size);
  if (b->grid == NULL) {
    exit(NULL_POINTER_ERR);
  }

  for (int i = 0; i < size * size; i++) {
    b->grid[i] = EMPTY_INTERSECTION;
  }

  return b;
}

/**
 * Deletes a board.
 * @param b The board to delete.
*/
void board_delete(board* b) {
  if (b == NULL) {
    exit(NULL_POINTER_ERR);
  }

  free(b->grid);
  free(b);
}

/**
 * Prints a board.
 * @param b The board to print.
 * @param in_place Whether to print the board in place.
*/
void board_print(board* b, bool in_place) {
    if (in_place) {
        clear();
    }

    for (int i = b->size - 1; i >= 0; i--) {
        printf("%2d ", i + 1);
        for (int j = 0; j < b->size; j++) {
            switch (board_get(b, j, i)) {
                case BLACK_STONE:
                    if (j == b->size - 1)
                        printf("\u25CF");
                    else
                        printf("\u25CF-");
                    break;
                case WHITE_STONE:
                    if (j == b->size - 1)
                        printf("\u25CB");
                    else
                        printf("\u25CB-");
                    break;
                default:
                    if (j == b->size - 1)
                        printf("+");
                    else
                        printf("+-");
                    break;
            }
        }

        printf("\n");
    }

    printf("   ");
    for (int i = 0; i < b->size - 1; i++) {
        printf("%c ", 'A' + i);
    }
    printf("%c", 'A' + b->size - 1);
    printf("\n");
}

/**
 * Converts a board coordinate to a formal coordinate.
 * @param b The board to get the formal coordinate from.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param formal_coord The formal coordinate to write to.
*/
unsigned char board_formal_coord(board* b, unsigned char x, unsigned char y, char* formal_coord) {
  if (x >= b->size || y >= b->size) {
    return COORDINATE_ERR;
  }

  formal_coord[0] = 'A' + x;
  formal_coord[1] = '0' + y;
  formal_coord[2] = '\0';

  return SUCCESS;
}

/**
 * Converts a formal coordinate to a board coordinate.
 * @param b The board to get the board coordinate from.
 * @param formal_coord The formal coordinate.
 * @param x The x coordinate to write to.
 * @param y The y coordinate to write to.
*/
unsigned char board_coord(board* b, const char* formal_coord, unsigned char* x, unsigned char* y) {
  if (formal_coord[0] < 'A' || formal_coord[0] > 'A' + b->size - 1) {
    return FORMAL_COORDINATE_ERR;
  }

  if (formal_coord[1] < '0' || formal_coord[1] > '0' + b->size - 1) {
    return FORMAL_COORDINATE_ERR;
  }

  *x = formal_coord[0] - 'A';
  *y = formal_coord[1] - '0';

  return SUCCESS;
}

/**
 * Gets the stone at a coordinate.
 * @param b The board to get the stone from.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return The stone at the coordinate.
*/
unsigned char board_get(board* b, unsigned char x, unsigned char y) {
  if (x > b->size || y > b->size) {
    printf("Coordinate error 1 \n");
    return COORDINATE_ERR;
  }

    return b->grid[y * b->size + x];
}

/**
 * Sets the stone at a coordinate.
 * @param b The board to set the stone on.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param stone The stone to set.
*/
void board_set(board* b, unsigned char x, unsigned char y, unsigned char stone) {
    if (stone != BLACK_STONE && stone != WHITE_STONE) {
        exit(STONE_TYPE_ERR);
    }

    if (x > b->size || y > b->size) {
        printf("Coordinate error 2 \n");
        exit(COORDINATE_ERR);
    }

    b->grid[y * b->size + x] = stone;
}

/**
 * Checks if a board is full.
 * @param b The board to check.
 * @return Whether the board is full.
*/
bool board_is_full(board* b) {
  for (int i = 0; i < b->size * b->size; i++) {
    if (b->grid[i] == EMPTY_INTERSECTION) {
      return false;
    }
  }

  return true;
}