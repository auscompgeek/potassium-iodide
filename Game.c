// Dominic He
// Matthew Solomonson
// David Vo
// Shravan Jeevan
// 17/05/2014
// ADT functions for KI

#include "Game.h"

#define NO_OF_DISCIPLINES 6
#define NO_OF_REGIONS 19
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
    int students[NO_OF_DISCIPLINES];
    int arcCount;
    int campusCount;
    int gO8Count;
    ARC arcs[ARC_LIMIT];
    vertex campuses[CAMPUS_LIMIT];
    vertex gO8Campuses[GO8_CAMPUS_LIMIT];
} uni;

struct _game {
    int turnNumber; // Calculate whoseTurn from this
    int regionDiscipline[NO_OF_REGIONS];
    int regionDiceValue[NO_OF_REGIONS];
    uni unis[NUM_UNIS];

    int mostPublications;
    int mostARCs;
}

// MAP FUNCTIONS: use these to get info on the map

// Given the LRB path of a vertex, returns its coord value
// Returns NULL if invalid path
static void arcToCoord(path arcPath, ARC destinationArc) {

}

// Given the LRB path of a vertex, 
// returns the coords of the 2 adjacent vertexes
// Returns NULL if invalid path
static vertex vertexToCoord(path vertexPath) {
    return NULL;
}

// Given the coords of a vertex, sets vertexPath to a converted
// LRB path char array
// sets to NULL if invalid coords
static void vertexToPath(vertex vertexLocation, path vertexPath); {

}

// Given the coords of an ARC's adjacent vertexes
// sets vertexPath to a converted LRB path array
// sets to NULL if invalid coords
static void arcToPath(ARC arcCoord, path arcPath) {

}

// Given a region's ID, sets vertexCoords to the vertex coords
// that belong to the region
static void vertexesofRegion(int regionID, vertex vertexCoords[6]) {

}

// Given a vertex and the previous vertex in a path
// finds the next vertex given by the single direction of the
// LRB path
// returns NULL if the vertex ends up in the sea
static vertex nextVertex(vertex current, vertex previous, char direction) {
    return NULL;
}

// Finds the adjacent vertexes around a vertex
// If less than 3 adjacent, sets NULL to 2nd/3rd element
static void adjacentVertexes(vertex current, vertex adjacents[3]); {

}

// INTERNAL FUNCTIONS: label as static if you decide to implement 
// internal functions to use for the ADT functions.

// ADT FUNCTIONS

// Shravan
Game newGame (int discipline[], int dice[]) {
    return NULL;
}

void disposeGame (Game g) {

}

void throwDice (Game g, int diceScore) {

}

int getDiscipline (Game g, int regionID) {
    // TODO
    return -1;
}

int getDiceValue (Game g, int regionID) {
    // TODO
    return 0;
}

int getMostARCs (Game g) {
    // TODO
    return 0;
}

// Matthew
int getMostPublications (Game g) {
    // TODO
    return 0;
}

int getTurnNumber (Game g) {
    // TODO
    return 0;
}

int getWhoseTurn (Game g) {
    // TODO
    return 0;
}

int getCampus(Game g, path pathToVertex) {
    // TODO
    return 0;
}

int getARC(Game g, path pathToEdge) {
    // TODO
    return 0;
}

// David
int isLegalAction(Game g, action a) {
    int code = a.actionCode;
    int player = g->playerTurn;
    int result;

    // no actions are valid in Terra Nullius
    if (getTurnNumber(g) == -1) {
        result = FALSE;
    } else if (code == PASS) {
        result = TRUE;
    } else if (code == BUILD_CAMPUS) {
        // TODO
    } else if (code == BUILD_G08) {
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
        result = getStudents(g, player, a.disciplineFrom) <=
            getExchangeRate(g, player, a.disciplineFrom, a.disciplineTo);
    } else {
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
    return g->unis[player - 1].g08Count;
}

int getCampuses(Game g, int player) {
    return g->unis[player - 1].campusCount;
}

// Dominic
void makeAction (Game g, action a) {
    int code = a.actionCode;
    int player = getWhoseTurn(g);
    int rate;
    int highestCount;
    if (a.actionCode == BUILD_CAMPUS) {
        // TODO
    } else if (code == BUILD_G08) {
        // TODO
    } else if (code == OBTAIN_ARC) {
        g->unis[player].arcs[g->unis[player].arcCount]
            = arcToCoord(a.destination);
        g->unis[player].arcCount++;
        highestCount = g->unis[g->mostARCs - 1].arcCount;
        if (g->unis[player].arcCount > highestCount) {
            g->mostARCs = player;
        }

        g->unis[player - 1].students[STUDENT_BPS]--;
        g->unis[player - 1].students[STUDENT_BQN]--;

    } else if (code == OBTAIN_PUBLICATION) {
        g->unis[player - 1].publicationCount++;
        highestCount = 
            g->unis[g->mostPublications - 1].publicationCount;
        if (g->unis[player - 1].publicationCount > highestCount) {
            g->mostPublications = player;
        }

        g->unis[player - 1].students[STUDENT_MJ]--;
        g->unis[player - 1].students[STUDENT_MTV]--;
        g->unis[player - 1].students[STUDENT_MMONEY]--;

    } else if (code == OBTAIN_IP_PATENT) {
        g->unis[player - 1].patentCount++;
        highestCount = g->unis[g->mostPatents - 1].patentCount;

        g->unis[player - 1].students[STUDENT_MJ]--;
        g->unis[player - 1].students[STUDENT_MTV]--;
        g->unis[player - 1].students[STUDENT_MMONEY]--;

    } else if (code == RETRAIN_STUDENTS) {
        rate = getExchangeRate(g, getWhoseTurn(g), 
            a.disciplineFrom, a.disciplineTo);
        g->unis[player - 1].students[a.disciplineFrom] -= rate;
        g->unis[player - 1].students[a.disciplineTo]++;
    }
}

int getIPs (Game g, int player) {
    return g->unis[player - 1].patentCount;
}

int getPublications (Game g, int player) {
    return g->unis[player - 1].publicationCount;
}

int getStudents (Game g, int player, int discipline) {
    return g->unis[player - 1].students[discipline];
}

int getExchangeRate (Game g, int player, 
                     int disciplineFrom, int disciplineTo) {
    // TODO
    return 3;
}
