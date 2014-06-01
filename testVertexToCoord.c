#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Game.h"

#define LEFT 'L'
#define RIGHT 'R'
#define BACK 'B'
#define END_PATH '\0'

typedef struct _vertex {
    int x, y;
} vertex;

static int isValidVertex(vertex check);
static vertex vertexToCoord(path vertexPath);
static vertex nextVertex(vertex current, vertex previous,
                         char direction);

int main(int argc, char *argv[])
{
    path vertexPath;
    vertex vertexCoord;
    scanf("%s", vertexPath);
    printf("vertexPath: %s\n", vertexPath);
    vertexCoord = vertexToCoord(vertexPath);
    printf("{%d, %d}\n", vertexCoord.x, vertexCoord.y);
    return EXIT_SUCCESS;
}

static int isValidVertex(vertex check) {
    int isValid = FALSE;
    int x, y;
    x = check.x;
    y = check.y;
    if (x == 0 || x == 5) {
        if (y >= 2 && y <= 8) {
            isValid = TRUE;
        }
    } else if (x == 1 || x == 4) {
        if (y >= 1 && y <= 9) {
            isValid = TRUE;
        }
    } else if (x == 2 || x == 3) {
        if (y >= 0 && y <= 10) {
            isValid = TRUE;
        }
    }
    return isValid;
}

// Given the LRB path of a vertex,
// returns the coords of the 2 adjacent vertices
// Returns {-1, -1} if invalid path
static vertex vertexToCoord(path vertexPath) {
    vertex current;
    vertex previous, tempPrevious;
    int i = 0;
    int invalid = FALSE;

    // set previous to path starting point
    previous.x = 2;
    previous.y = 10;

    if (vertexPath[i] == END_PATH) {
        current = previous;
    } else {
        if (vertexPath[i] == LEFT) {
            current.x = 3;
            current.y = 10;
        } else if (vertexPath[i] == RIGHT) {
            current.x = 2;
            current.y = 9;
        } else {
            invalid = TRUE;
        }
        i++;
        // Follow path
        while (!invalid && vertexPath[i] != END_PATH) {
            if (vertexPath[i] != LEFT &&
                vertexPath[i] != RIGHT &&
                vertexPath[i] != BACK) {
                invalid = TRUE;
            } else {
                tempPrevious = current;
                current = nextVertex(current, previous, vertexPath[i]);
                previous = tempPrevious;
                if (!isValidVertex(current)) {
                    invalid = TRUE;
                }
                i++;
            }
        }
        if (invalid == TRUE) {
            current.x = -1;
            current.y = -1;
        }
    }
    return current;
}

// Given a vertex and the previous vertex in a path
// finds the next vertex given by the single direction of the
// LRB path
// returns {-1, -1} if the vertex ends up in the sea
static vertex nextVertex(vertex current, vertex previous,
                         char direction) {
    vertex next;
    if (direction == BACK) {
        next = previous;
    } else if (previous.y == current.y) { // facing horizontally
        next.x = current.x;
        if (previous.x < current.x) { // facing east
            if (direction == LEFT) {
                next.y = current.y + 1;
            } else { // direction == RIGHT
                next.y = current.y - 1;
            }
        } else { // facing west
            if (direction == LEFT) {
                next.y = current.y - 1;
            } else {
                next.y = current.y + 1;
            }
        }
    } else { // facing vertically
        if ((current.x + current.y) % 2 == 0) {
            if (previous.y < current.y) { // facing northeast
                if (direction == LEFT) {
                    next.x = current.x;
                    next.y = current.y + 1;
                } else {
                    next.x = current.x + 1;
                    next.y = current.y;
                }
            } else { // facing southeast
                if (direction == LEFT) {
                    next.x = current.x + 1;
                    next.y = current.y;
                } else {
                    next.x = current.x;
                    next.y = current.y - 1;
                }
            }
        } else {
            if (previous.y < current.y) { // facing northwest
                if (direction == LEFT) {
                    next.x = current.x - 1;
                    next.y = current.y;
                } else {
                    next.x = current.x;
                    next.y = current.y + 1;
                }
            } else { // facing southwest
                if (direction == LEFT) {
                    next.x = current.x;
                    next.y = current.y - 1;
                } else {
                    next.x = current.x - 1;
                    next.y = current.y;
                }
            }
        }
    }
    return next;
}

