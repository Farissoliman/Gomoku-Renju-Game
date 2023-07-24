CC = gcc
CFLAGS = -Wall -std=c99 -g

all: gomoku renju replay

gomoku: io.c board.c game.c gomoku.c
	$(CC) $(CFLAGS) io.c board.c game.c gomoku.c -o gomoku

renju: io.c board.c game.c renju.c
	$(CC) $(CFLAGS) io.c board.c game.c renju.c -o renju

replay: io.c board.c game.c replay.c
	$(CC) $(CFLAGS) io.c board.c game.c replay.c -o replay

clean:
	rm -f gomoku renju replay