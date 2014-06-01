CC = gcc
CC_FLAGS = -Wall -Werror -O -std=c99
GAME = Game.c

all: test

clean:
	rm -f runGame testGame

run: runGame
	./runGame

runGame: runGame.c $(GAME)
	$(CC) $(CC_FLAGS) runGame.c $(GAME) -o runGame

# RUN ALL THE TESTS
test: testNextVertex testVertexToCoord testVerticesOfRegion testGame
	./testNextVertex
	./testVertexToCoord
	./testVerticesOfRegion
	./testGame

testGame: testGame.c $(GAME)
	$(CC) $(CC_FLAGS) testGame.c $(GAME) -o testGame
