CC = gcc
CC_FLAGS = -Wall -Werror -O -std=c99

all: test

run: runGame
	./runGame

runGame: runGame.c Game.c
	$(CC) $(CC_FLAGS) -o runGame runGame.c Game.c

test: testGame
	./testGame

testGame: testGame.c Game.c
	$(CC) $(CC_FLAGS) -o testGame testGame.c Game.c
