/*
 * Game.c - implementation of struct _game
 * by David Vo, Dominic He, Matthew Solomonson, Shravan Jeevan
 * Copyleft 2014
 * All wrongs reserved
 */

#include <stdlib.h>
#include "Game.h"

#define NUM_DISCIPLINES 6
#define ARC_LIMIT 68
#define CAMPUS_LIMIT 50
#define GO8_CAMPUS_LIMIT 8

typedef struct _vertex {
    int x, y;
} vertex;

typedef vertex ARC[2];

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
    int turnNumber; // Calculate whoseTurn from this
    int regionDiscipline[NUM_REGIONS];
    int regionDiceValue[NUM_REGIONS];
    uni unis[NUM_UNIS];

    int mostPublications;
    int mostARCs;
};

// MAP FUNCTIONS: use these to get info on the map

// Given the LRB path of a vertex, returns its coord value
// Returns NULL if invalid path
static void arcToCoord(path arcPath, ARC destinationArc) {

}

// Given the LRB path of a vertex,
// returns the coords of the 2 adjacent vertexes
// Returns NULL if invalid path
static vertex vertexToCoord(path vertexPath) {
    vertex v;
    return v;
}

// Given the coords of a vertex, sets vertexPath to a converted
// LRB path char array
// sets to NULL if invalid coords
static void vertexToPath(vertex vertexLocation, path vertexPath) {

}

// Given the coords of an ARC's adjacent vertexes
// sets vertexPath to a converted LRB path array
// sets to NULL if invalid coords
static void arcToPath(ARC arcCoord, path arcPath) {

}

// Given a region's ID, sets vertexCoords to the vertex coords
// that belong to the region
static void verticesOfRegion(int regionID, vertex vertexCoords[6]) {

}

// Given a vertex and the previous vertex in a path
// finds the next vertex given by the single direction of the
// LRB path
// returns NULL if the vertex ends up in the sea
static vertex nextVertex(vertex current, vertex previous,
                         char direction) {
    vertex v;
    return v;
}

// Finds the adjacent vertexes around a vertex
// If less than 3 adjacent, sets NULL to 2nd/3rd element
static void adjacentVertexes(vertex current, vertex adjacents[3]) {

}

// INTERNAL FUNCTIONS: label as static if you decide to implement
// internal functions to use for the ADT functions.
static int compareVertex(vertex vertex1, vertex vertex2) {
    return (vertex1.x == vertex2.x) && (vertex1.y == vertex2.y);
}

static int vertexInPlayer(uni *playerUni, vertex campus) {
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

// ADT FUNCTIONS

// Shravan
Game newGame(int discipline[], int dice[]) {
    // TODO
    return NULL;
}

void disposeGame(Game g) {
    // TODO
}

int getDiscipline(Game g, int regionID) {
    // TODO
    return -1;
}

int getDiceValue(Game g, int regionID) {
    // TODO
    return 0;
}

int getMostARCs(Game g) {
    // TODO
    return 0;
}

// Matthew
void throwDice(Game g, int diceScore) {
    // TODO
}

int getMostPublications(Game g) {
    // TODO
    return 0;
}

int getTurnNumber(Game g) {
    return g->turnNumber;
}

int getWhoseTurn(Game g) {
    // WIP
    if (g->turnNumber == -1) {
        currentTurn = -1;
    } else if (g->turnNumber > 0) {
        if ((getTurnNumber(g) % 3) == 0) {
            currentTurn = 0;
        }
        if ((getTurnNumber(g) % 2) == 0) ||
           ((getTurnNumber(g) % 2) == 5) {
            currentTurn = 1;
        }
        if ((getTurnNumber(g) % 3) == 0) {
            currentTurn = 2;
        }


    }
    return currentTurn;
}

int getCampus(Game g, path pathToVertex) {
    // TODO
    //while
    return 0;
}

int getARC(Game g, path pathToEdge) {
    // TODO
    return 0;
}

// David
int isLegalAction(Game g, action a) {
    int code = a.actionCode;
    int player = getWhoseTurn(g);
    int result;
    int discipFrom;

    // no actions are valid in Terra Nullius
    if (getTurnNumber(g) == -1) {
        result = FALSE;
    } else if (code == PASS) {
        result = TRUE;
    } else if (code == BUILD_CAMPUS) {
        // TODO
    } else if (code == BUILD_GO8) {
        // TODO
    } else if (code == OBTAIN_ARC) {
        // TODO
    } else if (code == START_SPINOFF) {
        // TODO
    } else if (code == OBTAIN_PUBLICATION) {
        result = FALSE;
    } else if (code == OBTAIN_IP_PATENT) {
        result = FALSE;
    } else if (code == RETRAIN_STUDENTS) {
        discipFrom = a.discliplineFrom;
        if (discipFrom == STUDENT_THD ||
           discipFrom < 0 || discipFrom >= NUM_DISCIPLINES) {
            result = FALSE;
        } else {
            result = getStudents(g, player, discipFrom) <=
                getExchangeRate(g, player, discipFrom, a.disciplineTo);
        }
    } else {
        // action code is out of range
        result = FALSE;
    }
    return result;
}

int getKPIpoints(Game g, int player) {
    // Should we keep a rolling count of KPI points, neglecting the
    // most ARCs and publications prestige awards? Would be easier.
    //int points = g->unis[player - 1].kpiPoints;

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
    return g->unis[player - 1].campusCount;
}

// Dominic
void makeAction(Game g, action a) {
    int code = a.actionCode;
    int player = getWhoseTurn(g);
    int rate;
    int currentCount;
    int highestCount;
    uni *playerUni = &(g->unis[player - 1]);
    ARC obtainedArc;

    if (code == BUILD_CAMPUS) {
        // TODO
    } else if (code == BUILD_GO8) {
        // TODO
    } else if (code == OBTAIN_ARC) {
        arcToCoord(a.destination, obtainedArc);
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
        highestCount =
            g->unis[g->mostPublications - 1].publicationCount;
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
        rate = getExchangeRate(g, player,
            a.disciplineFrom, a.disciplineTo);
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
    uni *playerUni = &(g->unis[player - 1]);
    int rate;
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
    if (vertexInPlayer(playerUni, retraincenter1) ||
       vertexInPlayer(playerUni, retraincenter2)) {
        rate = 2;
    } else {
        rate = 3;
    }
    return rate;
}
