CC = gcc
CFLAGS = -Wall -Werror -O -std=c99
GAME = Game.c

.PHONY: all clean run test

all: test

clean:
	rm -f runGame testGame
	rm -f testNextVertex testVertexToCoord testVerticesOfRegion

run: runGame
	./runGame

runGame: runGame.c $(GAME)
	$(CC) $(CFLAGS) runGame.c $(GAME) -o runGame

# RUN ALL THE TESTS
test: testNextVertex testVertexToCoord testVerticesOfRegion testGame
	./testNextVertex
	./testVertexToCoord
	./testVerticesOfRegion
	./testGame

testGame: testGame.c $(GAME)
	$(CC) $(CFLAGS) testGame.c $(GAME) -o testGame
