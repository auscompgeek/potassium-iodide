ifeq "$(origin CC)" "default"
CC      = gcc
endif

CFLAGS ?= -Wall -Werror -O -std=c99
GAME   ?= Game.c
TESTGAME ?= testGame.c

.PHONY: all clean run test test-all

all: test

clean:
	rm -f Game.o runGame.o testGame.o
	rm -f runGame testGame
	rm -f testNextVertex testVertexToCoord testVerticesOfRegion

run: runGame
	./runGame

runGame: runGame.o $(GAME:.c=.o)

# RUN ALL THE TESTS
test: testGame
	./testGame

test-all: testNextVertex testVertexToCoord testVerticesOfRegion testGame
	./testNextVertex
	./testVertexToCoord
	./testVerticesOfRegion
	./testGame

testGame: $(TESTGAME:.c=.o) $(GAME:.c=.o)
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@
