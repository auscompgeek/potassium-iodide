// Dominic He
// David Vo
// Matthew Solomonson
// Shravan Jeevan
// 24/05/2014
// runGame.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Game.h"

#define END_PATH '\0'

static void pass(Game g);
static void buildCampus(Game g, path location);
static void buildGO8(Game g, path location);
static void obtainArc(Game g, path location);
static void startSpinoff(Game g);
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
        printf("%dxThD %dxBPS %dxB? %dxMJ %dxMTV %dxM$\n",
            getStudents(g, i, STUDENT_THD),
            getStudents(g, i, STUDENT_BPS),
            getStudents(g, i, STUDENT_BQN),
            getStudents(g, i, STUDENT_MJ),
            getStudents(g, i, STUDENT_MTV),
            getStudents(g, i, STUDENT_MMONEY));
        i++;
    }
    printf("KPI Points: UNI_A %d, UNI_B %d, UNI_C %d\n",
           getKPIpoints(g, UNI_A),
           getKPIpoints(g, UNI_B),
           getKPIpoints(g, UNI_C));
    printf("Campuses: UNI_A %d, UNI_B %d, UNI_C %d\n",
           getCampuses(g, UNI_A),
           getCampuses(g, UNI_B),
           getCampuses(g, UNI_C));
    printf("GO8s: UNI_A %d, UNI_B %d, UNI_C %d\n",
           getGO8s(g, UNI_A), getGO8s(g, UNI_B), getGO8s(g, UNI_C));
    printf("ARCs: UNI_A %d, UNI_B %d, UNI_C %d\n",
           getARCs(g, UNI_A), getARCs(g, UNI_B), getARCs(g, UNI_C));
    printf("IP: UNI_A %d, UNI_B %d, UNI_C %d\n",
           getIPs(g, UNI_A), getIPs(g, UNI_B), getIPs(g, UNI_C));
    printf("Publications: UNI_A %d, UNI_B %d, UNI_C %d\n",
           getPublications(g, UNI_A),
           getPublications(g, UNI_B),
           getPublications(g, UNI_C));
}

static void pass(Game g) {
    action gameAction = {PASS};
    assert(isLegalAction(g, gameAction));
    makeAction(g, gameAction);
}

static void buildCampus(Game g, path location) {
    action gameAction = {BUILD_CAMPUS};
    int player = getWhoseTurn(g);
    int numCampuses = getCampuses(g, player);
    int kpiPoints = getKPIpoints(g, player);

    strncpy(gameAction.destination, location, PATH_LIMIT - 1);
    gameAction.destination[PATH_LIMIT - 1] = END_PATH;

    if (getCampus(g, location) != VACANT_VERTEX) {
        printf("Error: A campus already exists at %s.\n", location);
    } else if (isLegalAction(g, gameAction)) {
        printf("  * build campus: %s\n", location);
        makeAction(g, gameAction);

        assert(getCampuses(g, player) == numCampuses + 1);
        assert(getCampus(g, location) == player);
        assert(getKPIpoints(g, player) == kpiPoints + 10);
    } else {
        printf("Error: Cannot build a campus at %s.\n", location);
    }
}

static void buildGO8(Game g, path location) {
    action gameAction = {BUILD_GO8};
    int player = getWhoseTurn(g);
    int numNormalCampuses = getCampuses(g, player);
    int numGO8 = getGO8s(g, player);
    int kpiPoints = getKPIpoints(g, player);

    strncpy(gameAction.destination, location, PATH_LIMIT - 1);
    gameAction.destination[PATH_LIMIT - 1] = END_PATH;

    if (getCampus(g, location) != player) {
        printf("Error: Player does not have campus at %s.\n", location);
    } else if (isLegalAction(g, gameAction)) {
        printf("  * build GO8: %s\n", location);
        makeAction(g, gameAction);

        assert(getCampuses(g, player) == numNormalCampuses - 1);
        assert(getGO8s(g, player) == numGO8 + 1);
        // GO8 campus code = normal code + NUM_UNIS
        assert(getCampus(g, location) == player + NUM_UNIS);
        assert(getKPIpoints(g, player) == kpiPoints + 10);
    } else {
        puts("Error: Cannot build a Go8 campus.");
    }
}

static void obtainArc(Game g, path location) {
    action gameAction = {OBTAIN_ARC};
    int player = getWhoseTurn(g);
    int numARCs = getARCs(g, player);

    strncpy(gameAction.destination, location, PATH_LIMIT - 1);
    gameAction.destination[PATH_LIMIT - 1] = END_PATH;

    if (getARC(g, location) != NO_ONE) {
        printf("Error: There is already an ARC at %s.\n", location);
    } else if (isLegalAction(g, gameAction)) {
        printf("  * obtain ARC: %s\n", location);
        makeAction(g, gameAction);

        assert(getARC(g, location) == player);
        assert(getARCs(g, player) == numARCs + 1);
    } else {
        printf("Error: Cannot build an ARC at %s.\n", location);
    }
}

static void startSpinoff(Game g) {
    action gameAction = {START_SPINOFF};
    int count;
    int player = getWhoseTurn(g);

    if (!isLegalAction(g, gameAction)) {
        puts("Error: Not enough students to start a spinoff.");
    } else if (rand() % 2) {
        puts("  * start spinoff, obtain patent");
        count = getIPs(g, player);
        gameAction.actionCode = OBTAIN_IP_PATENT;
        makeAction(g, gameAction);
        assert(getIPs(g, player) == count + 1);
    } else {
        puts("  * start spinoff, obtain publication");
        count = getPublications(g, player);
        gameAction.actionCode = OBTAIN_PUBLICATION;
        makeAction(g, gameAction);
        assert(getPublications(g, player) == count + 1);
    }
}

static void retrain(Game g, int disciplineFrom, int disciplineTo) {
    int player = getWhoseTurn(g);
    int fromCount = getStudents(g, player, disciplineFrom);
    int toCount = getStudents(g, player, disciplineTo);

    action gameAction = {
        .actionCode = RETRAIN_STUDENTS,
        .disciplineFrom = disciplineFrom,
        .disciplineTo = disciplineTo
    };

    if (disciplineFrom != STUDENT_THD) {
        int rate = getExchangeRate(g, player, disciplineFrom, disciplineTo);

        if (fromCount >= rate) {
            printf("  * retrain: %d => %d\n", disciplineFrom, disciplineTo);

            assert(isLegalAction(g, gameAction));
            makeAction(g, gameAction);

            assert(getStudents(g, player, disciplineFrom) == fromCount - rate);
            assert(getStudents(g, player, disciplineTo) == toCount + 1);
        } else {
            printf("Error: Not enough students to retrain"
                    " (have %d, need %d).\n", fromCount, rate);
        }
    } else {
        puts("Error: ThD students cannot be retrained.");
    }
}
