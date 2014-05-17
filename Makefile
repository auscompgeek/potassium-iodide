CC = gcc

all: test

run: runGame
	./runGame

runGame: runGame.c Game.c
	$(CC) -Wall -Werror -O -std=c99 -o runGame runGame.c Game.c

test: testGame
	./testGame

testGame: testGame.c Game.c
	$(CC) -Wall -Werror -O -std=c99 -o testGame testGame.c Game.c
