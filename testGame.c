/*
 * testGame.c: test a Game implementation
 * by Dominic He, Matthew Solomonson, David Vo, Shravan Jeevan
 * Copyleft 2014
 * All wrongs reserved, including the blames.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Game.h"

#define DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS }
#define DICE_VALUES {9, 10, 8, 12, 6, 5, 3, 11, 3, \
                11, 4, 6, 4, 7, 9, 2, 8, 10, 5}
#define END_PATH '\0'

#define makeTurn(dice) nextTurn(g, &whoseTurn, &turnNum, dice)

static void checkStart(Game g);
static void playTurns(Game g);
static void nextTurn(Game g, int *whoseTurn, int *turnNum, int diceValue);

static void pass(Game g);
static void buildCampus(Game g, path location);
static void buildGO8(Game g, path location);
static void obtainArc(Game g, path location);
static void startSpinoff(Game g, int obtainPatent);
static void retrain(Game g, int disciplineFrom, int disciplineTo);

static void checkStudents(Game g, int player,
    int countThD, int countBPS, int countBQn,
    int countMJ, int countMTV, int countMMoney);

int main() {
    int disciplines[] = DISCIPLINES;
    int diceValues[] = DICE_VALUES;
    Game g = newGame(disciplines, diceValues);

    printf("Welcome to the Pokemon Centre.\n\n"
        "Checking the regions are initialised correctly...\n");

    int id = 0;
    while (id < NUM_REGIONS) {
        printf("%d\n", id);
        assert(getDiscipline(g, id) == disciplines[id]);
        assert(getDiceValue(g, id) == diceValues[id]);
        id++;
    }

    checkStart(g);
    playTurns(g);

    printf("Taking out the trash...\n\n");
    disposeGame(g);

    printf("All tests passed! Here's some awesome sauce. :3\n");
    return EXIT_SUCCESS;
}

static void checkStart(Game g) {
    printf("Checking the initial state...\n");

    assert(getTurnNumber(g) == -1);
    assert(getWhoseTurn(g) == NO_ONE);

    assert(getCampus(g, "RB") == CAMPUS_A);
    assert(getCampus(g, "RLRLRLRLRLL") == CAMPUS_A);
    assert(getCampus(g, "RRLRL") == CAMPUS_B);
    assert(getCampus(g, "RRLRLLRLRL") == CAMPUS_C);
    assert(getCampus(g, "LRLRL") == CAMPUS_C);
    assert(getCampus(g, "LRLRLRRLRL") == CAMPUS_B);

    assert(getARCs(g, UNI_A) == 0);
    assert(getARCs(g, UNI_B) == 0);
    assert(getARCs(g, UNI_C) == 0);

    assert(getGO8s(g, UNI_A) == 0);
    assert(getGO8s(g, UNI_B) == 0);
    assert(getGO8s(g, UNI_A) == 0);

    assert(getCampuses(g, UNI_A) == 2);
    assert(getCampuses(g, UNI_B) == 2);
    assert(getCampuses(g, UNI_C) == 2);

    assert(getIPs(g, UNI_A) == 0);
    assert(getIPs(g, UNI_B) == 0);
    assert(getIPs(g, UNI_C) == 0);

    assert(getPublications(g, UNI_A) == 0);
    assert(getPublications(g, UNI_B) == 0);
    assert(getPublications(g, UNI_C) == 0);

    checkStudents(g, UNI_A, 0, 3, 3, 1, 1, 1);
    checkStudents(g, UNI_B, 0, 3, 3, 1, 1, 1);
    checkStudents(g, UNI_C, 0, 3, 3, 1, 1, 1);

    printf("All clear!\n\n");
}

static void playTurns(Game g) {
    int whoseTurn = UNI_A;
    int turnNum = 0;

    printf("Let's play a game, shall we?\n"
        "* Turn 0...\n"
        "  * dice: 5\n"
        "  * player: 1\n");

    // 0 UNI_A
    throwDice(g, 5);
    assert(getWhoseTurn(g) == UNI_A);
    assert(getTurnNumber(g) == 0);

    checkStudents(g, UNI_B, 0, 4, 3, 1, 1, 1);
    obtainArc(g, "LRRLRLRLRLR");
    obtainArc(g, "LRRLRLRLRL");
    checkStudents(g, UNI_A, 0, 1, 1, 1, 1, 1);
    buildCampus(g, "LRRLRLRLR");
    checkStudents(g, UNI_A, 0, 0, 0, 0, 0, 1);

    // 1 UNI_B
    makeTurn(2);
    checkStudents(g, UNI_A, 0, 0, 0, 0, 0, 2);
    obtainArc(g, "LRLRLRRLRL");
    obtainArc(g, "LRLRLRRLR");
    checkStudents(g, UNI_B, 0, 2, 1, 1, 1, 1);
    buildCampus(g, "LRLRLRRL");
    checkStudents(g, UNI_B, 0, 1, 0, 0, 0, 1);

    // 2 UNI_C
    makeTurn(5);
    checkStudents(g, UNI_B, 0, 2, 0, 0, 0, 1);
    obtainArc(g, "RRLRLLRLRLL");
    obtainArc(g, "RRLRLLRLRLLL");
    checkStudents(g, UNI_C, 0, 1, 1, 1, 1, 1);
    buildCampus(g, "RRLRLLRLRLLL");
    checkStudents(g, UNI_C, 0, 0, 0, 0, 0, 1);

    // 3 UNI_A
    makeTurn(4);
    checkStudents(g, UNI_A, 0, 0, 1, 0, 0, 2);

    // 4 UNI_B
    makeTurn(5);
    checkStudents(g, UNI_B, 0, 3, 0, 0, 0, 1);
    checkStudents(g, UNI_C, 0, 1, 0, 0, 0, 1);

    // 5 UNI_C
    makeTurn(9);
    //checkStudents(g, UNI_C, 0, 3, 1, 0, 0, 1);

    // 6 UNI_A
    makeTurn(7);
    checkStudents(g, UNI_A, 2, 0, 1, 0, 0, 0);
    checkStudents(g, UNI_B, 1, 3, 1, 0, 0, 0);
    checkStudents(g, UNI_C, 1, 1, 0, 0, 0, 0);

    // 7 UNI_B
    makeTurn(8);
    //checkStudents(g, UNI_C, 1, 1, 0, 2, 2, 0);

    // 8 UNI_C
    makeTurn(9);
    checkStudents(g, UNI_B, 1, 3, 2, 0, 0, 0);

    // 9 UNI_A
    makeTurn(6);
    //checkStudents(g, UNI_A, 2, 0, 1, 0, 2, 0);

    // 10 UNI_B
    makeTurn(7);
    checkStudents(g, UNI_B, 1, 3, 2, 0, 0, 0);
    //checkStudents(g, UNI_C, 3, 1, 0, 0, 2, 0);
    //checkStudents(g, UNI_A, 2, 0, 1, 0, 2, 0);

    // 11 UNI_C
    makeTurn(9);
    checkStudents(g, UNI_B, 1, 3, 3, 0, 0, 0);

    // 12 UNI_A
    makeTurn(5);
    checkStudents(g, UNI_B, 1, 4, 3, 0, 0, 0);
    //checkStudents(g, UNI_C, 3, 2, 0, 0, 2, 0);

    // 13 UNI_B
    makeTurn(10);
    checkStudents(g, UNI_B, 1, 4, 4, 0, 0, 0);
    assert(getExchangeRate(g, UNI_B, STUDENT_BQN, STUDENT_MMONEY) == 2);
    retrain(g, STUDENT_BQN, STUDENT_MMONEY);
    checkStudents(g, UNI_B, 1, 4, 2, 0, 0, 1);
    assert(getExchangeRate(g, UNI_B, STUDENT_BPS, STUDENT_MMONEY) == 3);
    retrain(g, STUDENT_BPS, STUDENT_MMONEY);
    checkStudents(g, UNI_B, 1, 1, 2, 0, 0, 2);

    // 14 UNI_C
    makeTurn(11);
    //checkStudents(g, UNI_A, 2, 0, 1, 1, 2, 0);

    // 15 UNI_A
    makeTurn(8);

    /* Matt */

    // 16 UNI_B
    makeTurn(9);
    //checkStudents(g, UNI_B, 1, 1, 3, 0, 0, 2);
    //obtainArc(g, "LLRLRLLLL");
    //checkStudents(g, UNI_B, 1, 0, 2, 0, 0, 2);

    // 17 UNI_C
    makeTurn(4);
    // 18 UNI_A
    makeTurn(7);
    // 19 UNI_B
    makeTurn(7);
    // 20 UNI_C
    makeTurn(5);
    // 21 UNI_A
    makeTurn(12);
    // 22 UNI_B
    makeTurn(7);
    // 23 UNI_C
    makeTurn(6);
    // 24 UNI_A
    makeTurn(7);
    // 25 UNI_B
    makeTurn(7);
    // 26 UNI_C
    makeTurn(9);

    // 27 UNI_A
    makeTurn(11);

    // 28 UNI_B
    makeTurn(7);
    // 29 UNI_C
    makeTurn(6);
    // 30 UNI_A
    makeTurn(10);

    /* David */
    // XXX Matt hasn't finished his turns yet
    // 31 UNI_B
    makeTurn(7);
    obtainArc(g, "LRLRRLRLL");

    // 32 UNI_C
    makeTurn(9);

    // 33 UNI_A
    makeTurn(6);
    retrain(g, STUDENT_MJ, STUDENT_BPS);
    obtainArc(g, "LRRLRLRLR");

    // 34 UNI_B
    makeTurn(9);
    retrain(g, STUDENT_BQN, STUDENT_MTV);
    retrain(g, STUDENT_BQN, STUDENT_MJ);
    buildCampus(g, "LRRLRLLR");

    // 35 UNI_C
    makeTurn(7);
    // 36 UNI_A
    makeTurn(7);
    // 37 UNI_B
    makeTurn(6);
    // 38 UNI_C
    makeTurn(9);
    // 39 UNI_A
    makeTurn(4);
    // 40 UNI_B
    makeTurn(3);

    // 41 UNI_C
    makeTurn(5);
    retrain(g, STUDENT_MJ, STUDENT_MMONEY);
    startSpinoff(g, TRUE);

    // 42 UNI_A - the meaning of life
    makeTurn(9);

    // 43 UNI_B
    makeTurn(11);
    //obtainArc(g, "");//46
    //obtainArc(g, "");//60

    // 44 UNI_C
    makeTurn(10);
    retrain(g, STUDENT_BPS, STUDENT_BQN);
    //obtainArc(g, "");//46

    // 45 UNI_A
    makeTurn(7);
    retrain(g, STUDENT_MJ, STUDENT_BPS);
    //obtainArc(g, "");//50

    /* Dominic */
    // XXX Matt and David haven't finished their turns yet
    
    
    printf("You just lost the game.\n\n");
}

static void nextTurn(Game g, int *whoseTurn, int *turnNum, int diceValue) {
    pass(g);
    (*whoseTurn)++;
    if (*whoseTurn > NUM_UNIS) {
        *whoseTurn = UNI_A;
    }
    (*turnNum)++;

    printf("* Turn %d\n"
        "  * dice: %d\n"
        "  * player: %d\n",
        *turnNum, diceValue, *whoseTurn);

    throwDice(g, diceValue);
    assert(getTurnNumber(g) == *turnNum);
    assert(getWhoseTurn(g) == *whoseTurn);
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
    int player = getWhoseTurn(g);
    int numCampuses = getCampuses(g, player);
    int kpiPoints = getKPIpoints(g, player);

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

    assert(getCampuses(g, player) == numCampuses + 1);
    assert(getCampus(g, location) == player);
    assert(getKPIpoints(g, player) == kpiPoints + 10);
}

static void buildGO8(Game g, path location) {
    action gameAction;
    int i;
    int player = getWhoseTurn(g);
    int numNormalCampuses = getCampuses(g, player);
    int numGO8 = getGO8s(g, player);
    int kpiPoints = getKPIpoints(g, player);

    printf("  * build GO8: %s\n", location);

    assert(getCampus(g, location) == player);
    gameAction.actionCode = BUILD_GO8;
    i = 0;
    while (location[i] != END_PATH) {
        gameAction.destination[i] = location[i];
        i++;
    }
    gameAction.destination[i] = END_PATH;
    assert(isLegalAction(g, gameAction));
    makeAction(g, gameAction);

    assert(getCampuses(g, player) == numNormalCampuses - 1);
    assert(getGO8s(g, player) == numGO8 + 1);
    // GO8 campus code = normal code + 3
    assert(getCampus(g, location) == player + 3);
    assert(getKPIpoints(g, player) == kpiPoints + 10);
}

static void obtainArc(Game g, path location) {
    action gameAction;
    int i;
    int player = getWhoseTurn(g);
    int numARCs = getARCs(g, player);

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

    assert(getARC(g, location) == player);
    assert(getARCs(g, player) == numARCs + 1);
}

static void startSpinoff(Game g, int obtainPatent) {
    action gameAction;
    int count;
    int player = getWhoseTurn(g);

    printf("  * start spinoff; patent: %d\n", obtainPatent);

    gameAction.actionCode = START_SPINOFF;
    assert(isLegalAction(g, gameAction));
    if (obtainPatent) {
        count = getIPs(g, player);
        gameAction.actionCode = OBTAIN_IP_PATENT;
        makeAction(g, gameAction);
        assert(getIPs(g, player) == count + 1);
    } else {
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

    printf("  * retrain: %d => %d\n", disciplineFrom, disciplineTo);

    assert(fromCount >= getExchangeRate(g, player,
        disciplineFrom, disciplineTo));

    action gameAction;
    gameAction.actionCode = RETRAIN_STUDENTS;
    gameAction.disciplineFrom = disciplineFrom;
    gameAction.disciplineTo = disciplineTo;

    assert(isLegalAction(g, gameAction));
    makeAction(g, gameAction);

    assert(getStudents(g, player, disciplineFrom) ==
        fromCount - getExchangeRate(g, player, disciplineFrom, disciplineTo));
    assert(getStudents(g, player, disciplineTo) == toCount + 1);
}

static void checkStudents(
        Game g, int player,
        int countThD, int countBPS, int countBQn,
        int countMJ, int countMTV, int countMMoney) {
    assert(getStudents(g, player, STUDENT_THD) == countThD);
    assert(getStudents(g, player, STUDENT_BPS) == countBPS);
    assert(getStudents(g, player, STUDENT_BQN) == countBQn);
    assert(getStudents(g, player, STUDENT_MJ) == countMJ);
    assert(getStudents(g, player, STUDENT_MTV) == countMTV);
    assert(getStudents(g, player, STUDENT_MMONEY) == countMMoney);
}
