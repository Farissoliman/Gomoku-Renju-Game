/**
 * @file game.c
 * @author Faris Soliman
 * This file contains the implementation of the game struct and its functions.
 * It also contains the implementation of the functions used to check if the game is won or drawn.
 * It also contains the implementation of the functions used to place stones on the board
 * and used to create, delete, and print games. It contains all the methods for checking in game 
 * rules and updating the game.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "board.h"
#include "game.h"
#include "error-codes.h"
#include "io.h"

/**
 * Creates a new game.
 * @param board_size The size of the board.
 * @param game_type The type of the game.
 * @return The new game.
*/
game* game_create(unsigned char board_size, unsigned char game_type) {
    game* g = malloc(sizeof(game));
    if (g == NULL) {
        exit(NULL_POINTER_ERR);
    }

    g->board = board_create(board_size);
    g->type = game_type;
    g->stone = BLACK_STONE;
    g->state = GAME_STATE_PLAYING;
    g->winner = EMPTY_INTERSECTION;
    g->moves = malloc(sizeof(move) * 16);
    if (g->moves == NULL) {
        exit(NULL_POINTER_ERR);
    }

    g->moves_count = 0;
    g->moves_capacity = 16;

    return g;
}

/**
 * Deletes a game.
 * @param g The game to delete.
*/
void game_delete(game* g) {
    if (g == NULL) {
        exit(NULL_POINTER_ERR);
    }

    board_delete(g->board);
    free(g->moves);
    free(g);
}

/**
 * Checks if an input is valid.
 * @param input The input to check.
 * @return Whether the input is valid.
*/
bool static isValidInput(char* input, game* game) {
    if (strlen(input) == 0) {
        return false;
    }
    
    if (input[0] < 'A' || input[0] > game->board->size + 'A' - 1) {
        return false;
    }

    unsigned char y;
    sscanf(input + 1, "%hhu", &y);

    if (y <= 0 || y > game->board->size) {
        return false;
    }
    
    if (strcmp(input, "EOF") == 0) {
        return true;
    }
    
    if (board_get(game->board, input[0] - 'A', y - 1) != EMPTY_INTERSECTION) {
        return false;
    }
    
    return true;
}

bool static is_overline(game* g, unsigned char x, unsigned char y) {
    if (g->stone == WHITE_STONE) {
        return false;
    }

    int count = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;

    // check horizontal
    for (i = x - 4; i <= x + 4; i++) {
        if (i < 0 || i >= g->board->size) {
            continue;
        }
        if (board_get(g->board, i, y) == g->stone) {
            count++;
            if (count == 6) {
                return true;
            }
        } else {
            count = 0;
        }
    }
    
    // check vertical
    count = 0;
    for (j = y - 4; j <= y + 4; j++) {
        if (j < 0 || j >= g->board->size) {
            continue;
        }
        if (board_get(g->board, x, j) == g->stone) {
            count++;
            if (count == 6) {
                return true;
            }
        } else {
            count = 0;
        }
    }
    
    // check diagonal
    count = 0;
    for (k = -4; k <= 4; k++) {
        if (x + k < 0 || x + k >= g->board->size || y + k < 0 || y + k >= g->board->size) {
            continue;
        }
        if (board_get(g->board, x + k, y + k) == g->stone) {
            count++;
            if (count == 6) {
                return true;
            }
        } else {
            count = 0;
        }
    }
    
    // check diagonal (other way)
    count = 0;
    for (l = -4; l <= 4; l++) {
        if (x + l < 0 || x + l >= g->board->size || y - l < 0 || y - l >= g->board->size) {
            continue;
        }
        if (board_get(g->board, x + l, y - l) == g->stone) {
            count++;
            if (count == 6) {
                return true;
            }
        } else {
            count = 0;
        }
    }
    return false;
}

/**
 * This function checks if there is an open four.
 * @param g The game to check.
 * @param x The x coordinate of the stone.
 * @param y The y coordinate of the stone.
 * @return Whether there is an open four.
*/
bool static is_open_four(game* g, unsigned char x, unsigned char y) {
    if (g->stone == WHITE_STONE) {
        return false;
    }
    int count = 0;
    int i = 0;
    int j = 0;
    int totalIntersections = 0;

    // Check for open four horizontal
    for (i = x - 4; i <= x + 4; i++) {
        if (i < 0 || i >= g->board->size) {
            continue;
        }
        if (board_get(g->board, i, y) == g->stone) {
            count++;
            if (count == 4) {
                if (board_get(g->board, i + 1, y) == EMPTY_INTERSECTION && board_get(g->board, i - 5, y) == EMPTY_INTERSECTION) {
                    totalIntersections++;
                }            
            }
        } else {
            count = 0;
        }
    }
    
    // Check for open four vertical
    count = 0;
    for (j = y - 4; j <= y + 4; j++) {
        if (j < 0 || j >= g->board->size) {
            continue;
        }
        if (board_get(g->board, x, j) == g->stone) {
            count++;
            if (count == 4) {
                if (board_get(g->board, x, j + 1) == EMPTY_INTERSECTION && board_get(g->board, x, j - 5) == EMPTY_INTERSECTION) {
                    totalIntersections++;
                }
            }
        } else {
            count = 0;
        }
    }

    // Check for open four diagonal
    count = 0;
    for (i = -4; i <= 4; i++) {
        if (x + i < 0 || x + i >= g->board->size || y + i < 0 || y + i >= g->board->size) {
            continue;
        }
        if (board_get(g->board, x + i, y + i) == g->stone) {
            count++;
            if (count == 4) {
                if (board_get(g->board, x + i + 1, y + i + 1) == EMPTY_INTERSECTION && board_get(g->board, x + i - 5, y + i - 5) == EMPTY_INTERSECTION) {
                    totalIntersections++;
                }
            }
        } else {
            count = 0;
        }
    }

    // Check for open four diagonal (other way)
    count = 0;
    for (i = -4; i <= 4; i++) {
        if (x + i < 0 || x + i >= g->board->size || y - i < 0 || y - i >= g->board->size) {
            continue;
        }
        if (board_get(g->board, x + i, y - i) == g->stone) {
            count++;
            if (count == 4) {
                if (board_get(g->board, x + i + 1, y - i - 1) == EMPTY_INTERSECTION && board_get(g->board, x + i - 5, y - i + 5) == EMPTY_INTERSECTION) {
                    totalIntersections++;
                }
            }
        } else {
            count = 0;
        }
    }

    if (totalIntersections > 1) {
        return true;
    }
    return false;
}

/**
 * This function controls what happens in the game at each turn. 
 * @param g The game to update.
 * @return Whether the game is updated.
*/
bool game_update(game* g) {
    if (g->state != GAME_STATE_PLAYING) {
        return false;
    }

    char input[5];
    if (g->stone == BLACK_STONE) {
        printf("Black stone's turn, please enter a move: ");
    } else {
        printf("White stone's turn, please enter a move: ");
    }

    //Prompt until a valid input is entered
    while (fgets(input, 5, stdin)) {
        if (isValidInput(input, g)) {
            break;
        } else if (input[0] == 'E' && input[1] == 'O' && input[2] == 'F') {
            printf("The game is stopped.\n");
            g->state = GAME_STATE_STOPPED;
            return false;
        } else {
            printf("The coordinate you entered is invalid, please try again.\n");
        }
        if (g->stone == BLACK_STONE) {
            printf("Black stone's turn, please enter a move: ");
        } else {
            printf("White stone's turn, please enter a move: ");
        }
    }

    if (strlen(input) == 0) {
        printf("The game is stopped.\n");
        g->state = GAME_STATE_STOPPED;
        return false;
    }

    unsigned char value;
    sscanf(input + 1, "%hhu", &value);
    game_place_stone(g, input[0] - 'A', value - 1);

    return true;
}

/**
 * This function controls the game loop as the name suggests,
 * @param g The game to loop.
*/
void game_loop(game* g) {
    while (game_update(g)) {
        board_print(g->board, true);
    }

    if (g->state == GAME_STATE_FINISHED) {
        printf("Game concluded, %s won.\n", g->winner == BLACK_STONE ? "black" : "white");
    } else if (g->state == GAME_STATE_FORBIDDEN) {
        printf("Game concluded, black made a forbidden move, white won.\n");
    }
}

/**
 * Function resumes a new game from a saved file.
 * @param g The game to resume.
 * @return Whether the game is resumed.
*/
void game_resume(game* g) {

  if (g->state != GAME_STATE_STOPPED) {
    exit(RESUME_ERR);
  }

  if (g->type != GAME_FREESTYLE && g->type != GAME_RENJU) {
    exit(RESUME_ERR);
  }

  g->state = GAME_STATE_PLAYING;
  game_loop(g);
}

/**
 * This function replays a saved game. You should create a new game struct, and re-make moves saved in g, pause 1 second before making every move (this can be done by including a POSIX header unistd.h and call function sleep()). This function should act largely the same as game_loop() except it doesn't read user input, and prints extra information as specified in section Replay Saved Games.
*/
void game_replay(game* g) {
    if (g->state == GAME_STATE_PLAYING) {
        printf("Input error 1 \n");
        exit(INPUT_ERR);
    }
    
    if (g->type != GAME_FREESTYLE && g->type != GAME_RENJU) {
        printf("Input error 2 \n");
        exit(INPUT_ERR);
    }
    
    game* g2 = game_create(g->board->size, g->type);

    for (int i = 0; i < g->moves_count - 1; i++) {
        game_place_stone(g2, g->moves[i].x, g->moves[i].y);
        board_print(g2->board, true);

        printf("Moves:\n");
        for (int j = 0; j <= i; j++) {
            char move[15];
            if (g->moves[j].y + 1 >= 10) {
                sprintf(move, " %c%d", g->moves[j].x + 'A', g->moves[j].y + 1);
            } else {
                sprintf(move, "  %c%d", g->moves[j].x + 'A', g->moves[j].y + 1);
            }

            if (j % 2 == 0) {
                printf("%6s%4s", "Black:", move);
            } else {
                printf("  %6s%4s\n", "White:", move);
            }
        }
    }

    game_place_stone(g2, g->moves[g->moves_count - 1].x, g->moves[g->moves_count - 1].y);
    board_print(g2->board, true);

    if (g->state == GAME_STATE_FORBIDDEN) {
        printf("Game concluded, black made a forbidden move, white won.\n");
    } else if (g->state == GAME_STATE_FINISHED) {
        printf("Game concluded, %s won.\n", g->winner == BLACK_STONE ? "black" : "white");
    } else {
        printf("The game is stopped.\n");
    }

    printf("Moves:\n");
    for (int j = 0; j <= g->moves_count - 1; j++) {
        char move[15];
        if (g->moves[j].y + 1 >= 10) {
            sprintf(move, " %c%d", g->moves[j].x + 'A', g->moves[j].y + 1);
        } else {
            sprintf(move, "  %c%d", g->moves[j].x + 'A', g->moves[j].y + 1);
        }

        if (j % 2 == 0) {
            printf("%6s%4s", "Black:", move);
        } else {
            printf("  %6s%4s\n", "White:", move);
        }
    }

    if (g->moves_count % 2 != 0)
        printf("\n");
    game_delete(g2);
}

/**
 * This function checks if the game is a draw. It returns true if the board is full, false otherwise.
 * @param g The game to check.
 * @return Whether the game is a draw.
 * 
*/
bool static game_check_draw(game* g) {
    if (g->board->size * g->board->size == g->moves_count) {
        return true;
    }
    return false;
}

/**
 * This function checks if the game is won in freestyle mode. It returns true if the game is won, false otherwise.
 * @param g The game to check.
 * @param x The x coordinate of the stone.
 * @param y The y coordinate of the stone.
 * @return Whether the game is won.
*/
bool static game_check_win_freestyle(game* g, unsigned char x, unsigned char y) {
    int count = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    
    // check horizontal
    for (i = x - 4; i <= x + 4; i++) {
        if (i < 0 || i >= g->board->size) {
            continue;
        }
        if (board_get(g->board, i, y) == g->stone) {
            count++;
        } else {
            count = 0;
        }
        if (count == 5) {
            return true;
        }
    }
    
    // check vertical
    count = 0;
    for (j = y - 4; j <= y + 4; j++) {
        if (j < 0 || j >= g->board->size) {
            continue;
        }
        if (board_get(g->board, x, j) == g->stone) {
            count++;
        } else {
            count = 0;
        }
        if (count == 5) {
            return true;
        }
    }
    
    // check diagonal
    count = 0;
    for (k = -4; k <= 4; k++) {
        if (x + k < 0 || x + k >= g->board->size || y + k < 0 || y + k >= g->board->size) {
            continue;
        }
        if (board_get(g->board, x + k, y + k) == g->stone) {
            count++;
        } else {
            count = 0;
        }
        if (count == 5) {
            return true;
        }
    }
    
    // check diagonal (other way)
    count = 0;
    for (l = -4; l <= 4; l++) {
        if (x + l < 0 || x + l >= g->board->size || y - l < 0 || y - l >= g->board->size) {
            continue;
        }
        if (board_get(g->board, x + l, y - l) == g->stone) {
            count++;
        } else {
            count = 0;
        }
        if (count == 5) {
            return true;
        }
    }
    return false;
}

/**
 * This function checks if the game is won in renju mode. It returns true if the game is won, false otherwise.
 * @param g The game to check.
 * @param x The x coordinate of the stone.
 * @param y The y coordinate of the stone.
 * @return Whether the game is won.
*/
bool static game_check_win_renju(game* g, unsigned char x, unsigned char y) {
    int count = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;

    // check horizontal
    for (i = x - 4; i <= x + 4; i++) {
        if (i < 0 || i >= g->board->size) {
            continue;
        }
        if (board_get(g->board, i, y) == g->stone) {
            count++;
        } else {
            count = 0;
        }
        if (count == 5) {
            g->winner = g->stone;
            return true;
        }
    }
    
    // check vertical
    count = 0;
    for (j = y - 4; j <= y + 4; j++) {
        if (j < 0 || j >= g->board->size) {
            continue;
        }
        if (board_get(g->board, x, j) == g->stone) {
            count++;
        } else {
            count = 0;
        }
        if (count == 5) {
            g->winner = g->stone;
            return true;
        }
    }
    
    // check diagonal
    count = 0;
    for (k = -4; k <= 4; k++) {
        if (x + k < 0 || x + k >= g->board->size || y + k < 0 || y + k >= g->board->size) {
            continue;
        }
        if (board_get(g->board, x + k, y + k) == g->stone) {
            count++;
        } else {
            count = 0;
        }
        if (count == 5) {
            g->winner = g->stone;
            return true;
        }
    }
    
    // check diagonal (other way)
    count = 0;
    for (l = -4; l <= 4; l++) {
        if (x + l < 0 || x + l >= g->board->size || y - l < 0 || y - l >= g->board->size) {
            continue;
        }
        if (board_get(g->board, x + l, y - l) == g->stone) {
            count++;
        } else {
            count = 0;
        }
        if (count == 5) {
            g->winner = g->stone;
            return true;
        }
    }
    return false;
}

/**
 * This function checks if the game is won. It returns true if the game is won, false otherwise.
 * @param g The game to check.
 * @param x The x coordinate of the stone.
 * @param y The y coordinate of the stone.
 * @return Whether the game is won.
*/
bool static game_check_win(game* g, unsigned char x, unsigned char y) {
    if (g->type == GAME_FREESTYLE) {
        if (game_check_win_freestyle(g, x, y)) {
            return true;
        }
    } else if (g->type == GAME_RENJU) {
        if (game_check_win_renju(g, x, y)) {
            return true;
        }
    }
    return false;
}

/**
 * This function places a stone on the board. It returns true if the stone is placed, false otherwise.
 * It also checks if the game is won or drawn.
 * @param g The game to place the stone on.
 * @param x The x coordinate of the stone.
 * @param y The y coordinate of the stone.
 * @return Whether the stone is placed.
*/
bool game_place_stone(game* g, unsigned char x, unsigned char y) {
    if (g->state != GAME_STATE_PLAYING) {
        exit(INPUT_ERR);
    }
    
    if (g->type == GAME_FREESTYLE) {
        board_set(g->board, x, y, g->stone);
        g->moves[g->moves_count].x = x;
        g->moves[g->moves_count].y = y;
        g->moves_count++;
        if (g->moves_count == g->moves_capacity) {
            g->moves_capacity *= 2;
            g->moves = realloc(g->moves, sizeof(move) * g->moves_capacity);
            if (g->moves == NULL) {
                printf("Null error 1 \n");
                exit(NULL_POINTER_ERR);
            }
        }
    
        if (game_check_win(g, x, y)) {
            g->state = GAME_STATE_FINISHED;
            g->winner = g->stone;
            return true;
        }
    
        if (game_check_draw(g)) {
            g->state = GAME_STATE_FINISHED;
            return true;
        }
    
        g->stone = (g->stone == BLACK_STONE) ? WHITE_STONE : BLACK_STONE;
        return true;
    } else if (g->type == GAME_RENJU) {
        board_set(g->board, x, y, g->stone);
        g->moves[g->moves_count].x = x;
        g->moves[g->moves_count].y = y;
        g->moves_count++;
        if (g->moves_count == g->moves_capacity) {
            g->moves_capacity *= 2;
            g->moves = realloc(g->moves, sizeof(move) * g->moves_capacity);
            if (g->moves == NULL) {
                printf("Null error 1 \n");
                exit(NULL_POINTER_ERR);
            }
        }
        if (is_overline(g, x, y)) {
            g->state = GAME_STATE_FORBIDDEN;
            g->winner = WHITE_STONE;
            return true;
        }
    
        if (game_check_win(g, x, y)) {
            g->state = GAME_STATE_FINISHED;
            return true;
        }
    
        if (game_check_draw(g)) {
            g->state = GAME_STATE_FINISHED;
            return true;
        }
        if (is_open_four(g, x, y)) {
            g->state = GAME_STATE_FORBIDDEN;
            g->winner = WHITE_STONE;
            return true;
        }
    
        g->stone = (g->stone == BLACK_STONE) ? WHITE_STONE : BLACK_STONE;
        return true;
    }
    return false;
}