// Dominic He
// David Vo
// Matthew Solomonson
// Shravan Jeevan
// 24/05/2014
// runGame.c

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Game.h"

static void pass(Game g);
static void buildCampus(Game g, path location);
static void buildGO8(Game g, path location);
static void obtainArc(Game g, path location);
static void startSpinoff(Game g, int obtainPatent);
static void retrain(Game g, int disciplineFrom, int disciplineTo);

int main() {
    // TODO
    return EXIT_SUCCESS;
}

static void printInfo(Game g) {
    int i = UNI_A;
    while (i <= UNI_C) {
        if (i == UNI_A) {
            printf("UNI_A ");
        } else if (i == UNI_B) {
            printf("UNI_B ");
        } else {
            printf("UNI_C ");
        }
        // TODO print student numbers
    }
    printf("KPI Points\n"
           "UNI_A %d\n"
           "UNI_B %d\n"
           "UNI_C %d\n\n",
           getKPIpoints(g, UNI_A),
           getKPIpoints(g, UNI_B),
           getKPIpoints(g, UNI_C));
    printf("Campuses:\n"
           "UNI_A %d\n"
           "UNI_B %d\n"
           "UNI_C %d\n\n",
           getCampuses(g, UNI_A),
           getCampuses(g, UNI_B),
           getCampuses(g, UNI_C));
    printf("GO8s:\n"
           "UNI_A %d\n"
           "UNI_B %d\n"
           "UNI_C %d\n\n",
           getGO8s(g, UNI_A),
           getGO8s(g, UNI_B),
           getGO8s(g, UNI_C));
    printf("ARCs:\n"
           "UNI_A %d\n"
           "UNI_B %d\n"
           "UNI_C %d\n\n",
           getARCs(g, UNI_A),
           getARCs(g, UNI_B),
           getARCs(g, UNI_C));
    printf("IP:\n"
           "UNI_A %d\n"
           "UNI_B %d\n"
           "UNI_C %d\n\n",
           getIPs(g, UNI_A),
           getIPs(g, UNI_B),
           getIPs(g, UNI_C));
    printf("Publications:\n"
           "UNI_A %d\n"
           "UNI_B %d\n"
           "UNI_C %d\n",
           getPublications(g, UNI_A),
           getPublications(g, UNI_B),
           getPublications(g, UNI_C));
}

static void pass(Game g) {
    action gameAction;
    gameAction.actionCode = PASS;
    assert(isLegalAction(g, gameAction));
    makeAction(g, gameAction);
}

static void buildCampus(Game g, path location) {
    action gameAction;
    int i;
    int numCampuses = getCampuses(g, getWhoseTurn(g));
    int kpiPoints = getKPIpoints(g, getWhoseTurn(g));

    printf("  * build campus: %s\n", location);

    assert(getCampus(g, location) == VACANT_VERTEX);
    gameAction.actionCode = BUILD_CAMPUS;
    i = 0;
    while (location[i] != END_PATH) {
        gameAction.destination[i] = location[i];
        i++;
    }
    gameAction.destination[i] = END_PATH;
    assert(isLegalAction(g, gameAction));
    makeAction(g, gameAction);

    assert(getCampuses(g, getWhoseTurn(g)) == numCampuses + 1);
    assert(getCampus(g, location) == getWhoseTurn(g));
    assert(getKPIpoints(g, getWhoseTurn(g)) == kpiPoints + 10);
}

static void buildGO8(Game g, path location) {
    action gameAction;
    int i;
    int numNormalCampuses = getCampuses(g, getWhoseTurn(g));
    int numGO8 = getGO8s(g, getWhoseTurn(g));
    int kpiPoints = getKPIpoints(g, getWhoseTurn(g));

    printf("  * build GO8: %s\n", location);

    assert(getCampus(g, location) == getWhoseTurn(g));
    gameAction.actionCode = BUILD_GO8;
    i = 0;
    while (location[i] != END_PATH) {
        gameAction.destination[i] = location[i];
        i++;
    }
    gameAction.destination[i] = END_PATH;
    assert(isLegalAction(g, gameAction));
    makeAction(g, gameAction);

    assert(getCampuses(g, getWhoseTurn(g)) == numNormalCampuses - 1);
    assert(getGO8s(g, getWhoseTurn(g) == numGO8 + 1));
    // GO8 campuse code = normal code + 3
    assert(getCampus(g, location) == getWhoseTurn(g) + 3);
    assert(getKPIpoints(g, getWhoseTurn(g)) == kpiPoints + 10);
}

static void obtainArc(Game g, path location) {
    action gameAction;
    int i;
    int numARCs = getARCs(g, getWhoseTurn(g));

    printf("  * obtain ARC: %s\n", location);

    assert(getARC(g, location) == NO_ONE);
    gameAction.actionCode = OBTAIN_ARC;
    i = 0;
    while (location[i] != END_PATH) {
        gameAction.destination[i] = location[i];
        i++;
    }
    gameAction.destination[i] = END_PATH;
    assert(isLegalAction(g, gameAction));
    makeAction(g, gameAction);

    assert(getARC(g, location) == getWhoseTurn(g));
    assert(getARCs(g, getWhoseTurn(g)) == numARCs + 1);
}

static void startSpinoff(Game g, int obtainPatent) {
    action gameAction;
    int count;

    printf("  * start spinoff; patent: %d\n", obtainPatent);

    gameAction.actionCode = START_SPINOFF;
    assert(isLegalAction(g, gameAction));
    if (obtainPatent == TRUE) {
        count = getIPs(g, getWhoseTurn(g));
        gameAction.actionCode = OBTAIN_IP_PATENT;
        makeAction(g, gameAction);
        assert(getIPs(g, getWhoseTurn(g)) == count + 1);
    } else {
        count = getPublications(g, getWhoseTurn(g));
        gameAction.actionCode = OBTAIN_PUBLICATION;
        makeAction(g, gameAction);
        assert(getPublications(g, getWhoseTurn(g)) == count + 1);
    }
}

static void retrain(Game g, int disciplineFrom, int disciplineTo) {
    int fromCount = getStudents(g, getWhoseTurn(g), disciplineFrom);
    int toCount = getStudents(g, getWhoseTurn(g), disciplineTo);

    printf("  * retrain: %d => %d\n", disciplineFrom, disciplineTo);

    assert(fromCount >= getExchangeRate(g, getWhoseTurn(g),
        disciplineFrom, disciplineTo));

    action gameAction;
    gameAction.actionCode = RETRAIN_STUDENTS;
    gameAction.disciplineFrom = disciplineFrom;
    gameAction.disciplineTo = disciplineTo;
    assert(isLegalAction(g, gameAction));
    makeAction(g, gameAction);
    assert(getStudents(g, getWhoseTurn(g), disciplineFrom) ==
        fromCount - getExchangeRate(g, getWhoseTurn(g),
        disciplineFrom, disciplineTo));
    assert(getStudents(g, getWhoseTurn(g), disciplineTo) ==
        toCount + 1);
}