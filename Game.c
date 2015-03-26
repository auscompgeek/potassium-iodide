/*
 * Game.c - implementation of struct _game
 * by David Vo, Dominic He, Matthew Solomonson, Shravan Jeevan
 * Copyleft 2014
 * All wrongs reserved
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "Game.h"

#define NUM_DISCIPLINES 6
#define ARC_LIMIT 68
#define CAMPUS_LIMIT 50
#define GO8_CAMPUS_LIMIT 8
#define LEFT 'L'
#define RIGHT 'R'
#define BACK 'B'
#define END_PATH '\0'
#define REGIONS_TOPLEFT_X {0,0,0,1,1,1,1,2,2,2,2,2,3,3,3,3,4,4,4}
#define REGIONS_TOPLEFT_Y {8,6,4,9,7,5,3,10,8,6,4,2,9,7,5,3,8,6,4}

// Imagine the board on a Cartesian plane with the origin at the bottom left.
typedef struct _vertex {
    int x, y;
} vertex;

// ARCs have two values:
// 1. The location before the ARC from where it's going
// 2. The location after the ARC from location 1.
typedef vertex ARC[2];

typedef struct _uni * Uni;

// Stores all the values for the uni[x/3]
typedef struct _uni {
    int publicationCount;
    int patentCount;
    int students[NUM_DISCIPLINES];
    int arcCount;
    int campusCount;
    int gO8Count;
    ARC arcs[ARC_LIMIT];
    vertex campuses[CAMPUS_LIMIT];
    vertex gO8Campuses[GO8_CAMPUS_LIMIT];
} uni;

struct _game {
    int turnNumber;
    int regionDiscipline[NUM_REGIONS];
    int regionDiceValue[NUM_REGIONS];
    uni unis[NUM_UNIS];

    int mostPublications;
    int mostARCs;
};

// internal function definitions
static void arcPathToCoords(path arcPath, ARC destinationArc);
static vertex vertexPathToCoord(path vertexPath);
static void verticesOfRegion(int regionID, vertex vertexCoords[6]);
static vertex nextVertex(vertex current, vertex previous, char direction);
static void adjacentVertices(vertex current, vertex adjacents[3]);
static int compareVertex(vertex vertex1, vertex vertex2);
static int compareARC(ARC arc1, ARC arc2);
static int playerHasVertex(Uni playerUni, vertex campus);
static int playerHasGO8(Uni playerUni, vertex gO8Campus);
static int playerHasARC(Uni playerUni, ARC edge);
static int isValidVertex(vertex check);
static int isValidVertexPath(path vertexPath);
static int isValidARCPath(path arcPath);
static void adjacentARCs(ARC edge, ARC adjacents[4]);

// INTERNAL FUNCTIONS: label as static if you decide to implement
// internal functions to use for the ADT functions.

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

// Test for a valid vertex given a path
static int isValidVertexPath(path vertexPath) {
    return isValidVertex(vertexPathToCoord(vertexPath));
}

// Test for a valid ARC given a path
static int isValidARCPath(path arcPath) {
    printf("isvalidarcpath()\n");
    ARC arc;
    arcPathToCoords(arcPath, arc);
    return isValidVertex(arc[0]) && isValidVertex(arc[1]);
}

// Given the LRB path of a vertex, returns its coord value
// Returns NULL if invalid path
static void arcPathToCoords(path arcPath, ARC destinationArc) {
    printf("Arc path to coords()\n");
    path previousArc;
    strncpy(previousArc, arcPath, PATH_LIMIT);
    // Delete last character
    previousArc[strlen(previousArc) - 1] = END_PATH;

    // Find coords of adjacent vertices
    destinationArc[0] = vertexPathToCoord(previousArc);
    destinationArc[1] = vertexPathToCoord(arcPath);
}

// Given the LRB path of a vertex,
// returns the coords of the 2 adjacent vertices
// Returns {-1, -1} if invalid path
static vertex vertexPathToCoord(path vertexPath) {
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

// Given a region's ID, sets vertexCoords to the vertex coords
// that belong to the region
static void verticesOfRegion(int regionID, vertex vertexCoords[6]) {
    // Coords of top right of each region
    int regionsX[NUM_REGIONS] = REGIONS_TOPLEFT_X;
    int regionsY[NUM_REGIONS] = REGIONS_TOPLEFT_Y;
    int i = 0;
    while (i < 3) {
        vertexCoords[i].x = regionsX[regionID];
        vertexCoords[i + 3].x = regionsX[regionID] + 1;
        vertexCoords[i].y = regionsY[regionID] - i;
        vertexCoords[i + 3].y = regionsY[regionID] - i;
        i++;
    }
}

// Given a vertex and the previous vertex in a path
// finds the next vertex given by the single direction of the
// LRB path
// returns {-1, -1} if the vertex ends up in the sea
static vertex nextVertex(vertex current, vertex previous, char direction) {
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

// Finds the adjacent vertices around a vertex
// If less than 3 adjacent, sets 2nd/3rd vertices to {-1, -1}
static void adjacentVertices(vertex current, vertex adjacents[3]) {
    int i;
    adjacents[0].x = current.x;
    adjacents[0].y = current.y + 1;
    adjacents[1].x = current.x;
    adjacents[1].y = current.y - 1;
    if ((current.x + current.y) % 2 == 0) {
        adjacents[2].x = current.x + 1;
    } else {
        adjacents[2].x = current.x - 1;
    }
    adjacents[2].y = current.y;

    // Check that adjacent vertices are valid
    i = 0;
    while (i < 3) {
        if (!isValidVertex(adjacents[i])) {
            adjacents[i].x = -1;
            adjacents[i].y = -1;
        }
        i++;
    }
}

// Finds the adjacent ARCs around an ARC
static void adjacentARCs(ARC edge, ARC adjacents[4]) {
    int i, j;
    vertex adjVerts1[3], adjVerts2[3];
    adjacentVertices(edge[0], adjVerts1);
    adjacentVertices(edge[1], adjVerts2);
    i = 0;
    while (i < 3) {
        j = 0;
        if (!compareVertex(adjVerts1[i], edge[1])) {
            adjacents[j][0] = edge[0];
            adjacents[j][1] = adjVerts1[i];
            j++;
        }
        i++;
    }
    i = 0;
    while (i < 3) {
        j = 0;
        if (!compareVertex(adjVerts2[i], edge[0])) {
            adjacents[j][0] = edge[1];
            adjacents[j][1] = adjVerts2[i];
            j++;
        }
        i++;
    }
}

// Compares two vertices and returns true if they are equal.
static int compareVertex(vertex vertex1, vertex vertex2) {
    return (vertex1.x == vertex2.x) && (vertex1.y == vertex2.y);
}

// Compares two ARCs and returns true if they are equal.
static int compareARC(ARC arc1, ARC arc2){
    return (compareVertex(arc1[0], arc2[0]) &&
            compareVertex(arc1[1], arc2[1])) ||
           (compareVertex(arc1[0], arc2[1]) &&
            compareVertex(arc1[1], arc2[0]));
}

// Checks if a player has a campus at a vertex
static int playerHasVertex(Uni playerUni, vertex campus) {
    int i = 0;
    int result = FALSE;
    while (i < playerUni->campusCount && !result) {
        if (compareVertex(playerUni->campuses[i], campus)) {
            result = TRUE;
        }
        i++;
    }
    return result;
}

static int playerHasGO8(Uni playerUni, vertex gO8Campus) {
    int i = 0;
    int result = FALSE;
    while (i < playerUni->gO8Count && !result) {
        if (compareVertex(playerUni->gO8Campuses[i], gO8Campus)) {
            result = TRUE;
        }
        i++;
    }
    return result;
}

static int playerHasARC(Uni playerUni, ARC edge) {
    int result = FALSE;
    int i = 0;
    while (i < playerUni->arcCount && !result) {
        if (compareARC(playerUni->arcs[i], edge)) {
            result = TRUE;
        }
        i++;
    }
    return result;
}

// Returns TRUE if the player has an ARC adjacent to the given ARC
static int playerHasAdjacentARC(Uni playerUni, ARC edge) {
    int result = FALSE;
    int i = 0;
    ARC adjacents[4];
    adjacentARCs(edge, adjacents);

    while (i < 4 && !result) {
        if (playerHasARC(playerUni, adjacents[i])) {
            result = TRUE;
        }
        i++;
    }

    printf("Player has adj arc: %d", result);
    return result;
}

// Returns TRUE if there is a campus on a vertex adjacent to the given vertex
static int campusOnAdjacentVertex(Game g, vertex coord) {
    int result = FALSE;
    int i = 0;
    int player = UNI_A;
    int j;
    Uni playerUni;
    vertex adjacents[3];
    adjacentVertices(coord, adjacents);

    while (i < 3 && !result) {
        while (player <= NUM_UNIS && !result) {
            playerUni = &g->unis[player];
            j = 0;
            while (j < playerUni->campusCount && !result) {
                if (compareVertex(playerUni->campuses[j], adjacents[i])) {
                    result = TRUE;
                }
            }
            player++;
        }
        i++;
    }

    return result;
}

// ADT FUNCTIONS

/* Shravan and David */

Game newGame(int discipline[], int dice[]) {
    Game g = malloc(sizeof (struct _game));
    int i;
    Uni playerUni;

    g->turnNumber = -1;
    g->mostPublications = NO_ONE;
    g->mostARCs = NO_ONE;

    i = 0;
    while (i < NUM_REGIONS) {
        g->regionDiscipline[i] = discipline[i];
        g->regionDiceValue[i] = dice[i];
        i++;
    }

    i = 0;
    while (i < NUM_UNIS) {
        playerUni = &g->unis[i];
        playerUni->publicationCount = 0;
        playerUni->patentCount = 0;
        playerUni->arcCount = 0;
        playerUni->campusCount = 2;
        playerUni->gO8Count = 0;

        playerUni->students[STUDENT_THD] = 0;
        playerUni->students[STUDENT_BPS] = 3;
        playerUni->students[STUDENT_BQN] = 3;
        playerUni->students[STUDENT_MJ] = 1;
        playerUni->students[STUDENT_MTV] = 1;
        playerUni->students[STUDENT_MMONEY] = 1;

        i++;
    }

    // UNI_A
    g->unis[UNI_A - 1].campuses[0] = (vertex){2, 10};
    g->unis[UNI_A - 1].campuses[1] = (vertex){3, 0};
    // UNI_B
    g->unis[UNI_B - 1].campuses[0] = (vertex){0, 7};
    g->unis[UNI_B - 1].campuses[1] = (vertex){5, 3};
    // UNI_C
    g->unis[UNI_C - 1].campuses[0] = (vertex){0, 2};
    g->unis[UNI_C - 1].campuses[1] = (vertex){5, 8};

    return g;
}

void disposeGame(Game g) {
    free(g);
}

int getDiscipline(Game g, int regionID) {
    return g->regionDiscipline[regionID];
}

int getDiceValue(Game g, int regionID) {
    return g->regionDiceValue[regionID];
}

/* Shravan */

int getMostARCs(Game g) {
    return g->mostARCs;
}

/* Matthew */
int getMostPublications(Game g) {
    return g->mostPublications;
}

int getTurnNumber(Game g) {
    return g->turnNumber;
}

/* Matthew and David */

int getWhoseTurn(Game g) {
    return getTurnNumber(g) % 3 + 1;
}

int getCampus(Game g, path pathToVertex) {
    vertex campCoord = vertexPathToCoord(pathToVertex);
    int result = VACANT_VERTEX;
    int player = UNI_A;

    while (player <= NUM_UNIS && result == VACANT_VERTEX) {
        if (playerHasGO8(&g->unis[player - 1], campCoord)) {
            result = player + 3;
        } else if (playerHasVertex(&g->unis[player - 1], campCoord)) {
            result = player;
        }
        player++;
    }

    return result;
}

int getARC(Game g, path pathToEdge) {
    ARC arcCoords;
    int result = VACANT_ARC;
    int player = UNI_A;
    arcPathToCoords(pathToEdge, arcCoords);

    while (player <= NUM_UNIS && result == VACANT_ARC) {
        if (playerHasARC(&g->unis[player - 1], arcCoords)) {
            result = player;
        }
        player++;
    }

    return result;
}

/* David */

int isLegalAction(Game g, action a) {
    int code = a.actionCode;
    int player = getWhoseTurn(g);
    int result;
    int discipFrom, discipTo;
    ARC destinationArc;

    // no actions are valid in Terra Nullius
    if (getTurnNumber(g) == -1) {
        result = FALSE;
    } else if (code == PASS) {
        result = TRUE;
    } else if (code == BUILD_CAMPUS) {
        // TODO
        result =
            // check the player has enough students
            getStudents(g, player, STUDENT_BPS) >= 1 &&
            getStudents(g, player, STUDENT_BQN) >= 1 &&
            getStudents(g, player, STUDENT_MJ) >= 1 &&
            getStudents(g, player, STUDENT_MTV) >= 1 &&

            isValidVertexPath(a.destination) &&
            !campusOnAdjacentVertex(g, vertexPathToCoord(a.destination)) &&
            getCampus(g, a.destination) == VACANT_VERTEX;

    } else if (code == BUILD_GO8) {
        // TODO
        result =
            // check the player has enough students
            getStudents(g, player, STUDENT_MJ) >= 2 &&
            getStudents(g, player, STUDENT_MMONEY) >= 3 &&
            // check there are less than 8 GO8 campuses
            getGO8s(g, UNI_A) + getGO8s(g, UNI_B) + getGO8s(g, UNI_C) < 8 &&

            isValidVertexPath(a.destination) &&
            getCampus(g, a.destination) == player;

    } else if (code == OBTAIN_ARC) {
        printf("Checking legal obtain arc\n");
        arcPathToCoords(a.destination, destinationArc);
        result =
            // check the player has enough students
            getStudents(g, player, STUDENT_BQN) >= 1 &&
            getStudents(g, player, STUDENT_BPS) >= 1 &&

            isValidARCPath(a.destination) &&
            playerHasAdjacentARC(&g->unis[player - 1], destinationArc) &&
            getARC(g, a.destination) == VACANT_ARC;

    } else if (code == START_SPINOFF) {
        // the player merely needs to have enough players to start a spinoff
        result =
            getStudents(g, player, STUDENT_MJ) >= 1 &&
            getStudents(g, player, STUDENT_MTV) >= 1 &&
            getStudents(g, player, STUDENT_MMONEY) >= 1;

    } else if (code == OBTAIN_PUBLICATION || code == OBTAIN_IP_PATENT) {
        // The user isn't allowed to directly make these actions.
        // They must instead START_SPINOFF and then allow the runGame.c to
        // decide whether the spinoff results in a publication or patent.
        result = FALSE;

    } else if (code == RETRAIN_STUDENTS) {
        discipFrom = a.disciplineFrom;
        discipTo = a.disciplineTo;

        result =
            // ThDs can't be retrained
            discipFrom != STUDENT_THD &&
            // check the disciplines are in range
            discipFrom >= 0 && discipFrom < NUM_DISCIPLINES &&
            discipTo >= 0 && discipTo < NUM_DISCIPLINES &&
            // check the player has enough students to be retrained
            getStudents(g, player, discipFrom) >=
                getExchangeRate(g, player, discipFrom, discipTo);

    } else {
        // action code is out of range
        result = FALSE;
    }
    return result;
}

int getKPIpoints(Game g, int player) {
    int points =
        10 * getCampuses(g, player) +
        20 * getGO8s(g, player) +
        2 * getARCs(g, player) +
        10 * getIPs(g, player);

    if (getMostARCs(g) == player) {
        points += 10;
    }
    if (getMostPublications(g) == player) {
        points += 10;
    }

    return points;
}

int getARCs(Game g, int player) {
    return g->unis[player - 1].arcCount;
}

int getGO8s(Game g, int player) {
    return g->unis[player - 1].gO8Count;
}

int getCampuses(Game g, int player) {
    Uni playerUni = &g->unis[player - 1];
    return playerUni->campusCount - playerUni->gO8Count;
}

/* Dominic */

void makeAction(Game g, action a) {
    int code = a.actionCode;
    int player = getWhoseTurn(g);
    int rate;
    int currentCount;
    int highestCount;
    Uni playerUni = &(g->unis[player - 1]);
    ARC obtainedArc;
    vertex obtainedVertex;

    if (code == BUILD_CAMPUS) {
        obtainedVertex = vertexPathToCoord(a.destination);
        currentCount = playerUni->campusCount;
        playerUni->campuses[currentCount] = obtainedVertex;
        playerUni->campusCount++;

        playerUni->students[STUDENT_BPS]--;
        playerUni->students[STUDENT_BQN]--;
        playerUni->students[STUDENT_MJ]--;
        playerUni->students[STUDENT_MTV]--;

    } else if (code == BUILD_GO8) {
        // TODO
        obtainedVertex = vertexPathToCoord(a.destination);
        currentCount = playerUni->gO8Count;
        playerUni->gO8Campuses[currentCount] = obtainedVertex;
        playerUni->gO8Count++;

        playerUni->students[STUDENT_MJ] -= 2;
        playerUni->students[STUDENT_MMONEY] -= 3;

    } else if (code == OBTAIN_ARC) {
        arcPathToCoords(a.destination, obtainedArc);
        currentCount = playerUni->arcCount;
        playerUni->arcs[currentCount][0] = obtainedArc[0];
        playerUni->arcs[currentCount][1] = obtainedArc[1];
        playerUni->arcCount++;

        highestCount = g->unis[g->mostARCs - 1].arcCount;
        if (g->mostARCs == NO_ONE || currentCount + 1 > highestCount) {
            g->mostARCs = player;
        }

        playerUni->students[STUDENT_BPS]--;
        playerUni->students[STUDENT_BQN]--;

    } else if (code == OBTAIN_PUBLICATION) {
        playerUni->publicationCount++;
        highestCount = g->unis[g->mostPublications - 1].publicationCount;

        if (g->mostPublications == NO_ONE ||
           playerUni->publicationCount > highestCount) {
            g->mostPublications = player;
        }

        playerUni->students[STUDENT_MJ]--;
        playerUni->students[STUDENT_MTV]--;
        playerUni->students[STUDENT_MMONEY]--;

    } else if (code == OBTAIN_IP_PATENT) {
        playerUni->patentCount++;

        playerUni->students[STUDENT_MJ]--;
        playerUni->students[STUDENT_MTV]--;
        playerUni->students[STUDENT_MMONEY]--;

    } else if (code == RETRAIN_STUDENTS) {
        rate = getExchangeRate(g, player, a.disciplineFrom, a.disciplineTo);
        playerUni->students[a.disciplineFrom] -= rate;
        playerUni->students[a.disciplineTo]++;
    }
}

int getIPs(Game g, int player) {
    return g->unis[player - 1].patentCount;
}

int getPublications(Game g, int player) {
    return g->unis[player - 1].publicationCount;
}

int getStudents(Game g, int player, int discipline) {
    return g->unis[player - 1].students[discipline];
}

int getExchangeRate(Game g, int player,
                    int disciplineFrom, int disciplineTo) {
    Uni playerUni = &(g->unis[player - 1]);
    int rate;

    assert(disciplineFrom > STUDENT_THD && disciplineFrom < NUM_DISCIPLINES);
    assert(disciplineTo > STUDENT_THD && disciplineTo < NUM_DISCIPLINES);

    vertex retraincenter1;
    vertex retraincenter2;
    if (disciplineFrom == STUDENT_BPS) {
        retraincenter1.x = 1;
        retraincenter1.y = 2;
        retraincenter2.x = 1;
        retraincenter2.y = 1;
    } else if (disciplineFrom == STUDENT_BQN) {
        retraincenter1.x = 5;
        retraincenter1.y = 5;
        retraincenter2.x = 5;
        retraincenter2.y = 4;
    } else if (disciplineFrom == STUDENT_MJ) {
        retraincenter1.x = 4;
        retraincenter1.y = 2;
        retraincenter2.x = 5;
        retraincenter2.y = 2;
    } else if (disciplineFrom == STUDENT_MTV) {
        retraincenter1.x = 1;
        retraincenter1.y = 9;
        retraincenter2.x = 2;
        retraincenter2.y = 9;
    } else { // disciplineFrom == STUDENT_MMONEY
        retraincenter1.x = 3;
        retraincenter1.y = 9;
        retraincenter2.x = 4;
        retraincenter2.y = 9;
    }
    if (playerHasVertex(playerUni, retraincenter1) ||
       playerHasVertex(playerUni, retraincenter2)) {
        rate = 2;
    } else {
        rate = 3;
    }
    return rate;
}

void throwDice(Game g, int diceScore) {
    g->turnNumber++;
    int discipline;
    int i;
    Uni playerUni;
    vertex regVertices[6];
    int currentUni;
    int region = 0;
    while (region < NUM_REGIONS) {
        if (g->regionDiceValue[region] == diceScore) {
            // Find discpline of that region
            discipline = g->regionDiscipline[region];
            verticesOfRegion(region, regVertices);
            i = 0;
            while (i < 6) {
                // Find any campuses in that region
                currentUni = UNI_A;
                while (currentUni <= UNI_C) {
                    playerUni = &(g->unis[currentUni - 1]);
                    if (playerHasGO8(playerUni, regVertices[i])) {
                        playerUni->students[discipline] += 2;
                    } else if (playerHasVertex(playerUni, regVertices[i])) {
                        playerUni->students[discipline]++;
                    }
                    currentUni++;
                }
                i++;
            }
        }
        region++;
    }
}
