#include <stdio.h>
#include <stdlib.h>

#define LEFT 'L'
#define RIGHT 'R'
#define BACK 'B'

typedef struct _vertex {
    int x, y;
} vertex;

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

int main() {
        vertex current, previous, new;
        char direction;
        printf("\ndirection: ");
        scanf("%c", &direction);
        printf("current: ");
        scanf("%d %d", &(current.x), &(current.y));
        printf("\nprevious: ");
        scanf("%d %d", &(previous.x), &(previous.y));
        new = nextVertex(current, previous, direction);
        printf("New vertex: { %d, %d }\n", new.x, new.y);
    }
    return EXIT_SUCCESS;
}