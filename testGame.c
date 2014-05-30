/*
 * testGame.c: test a Game implementation
 * by Dominic He, Matthew Solomonson, David Vo, Shravan Jeevan
 * Copyleft 2014
 * All wrongs reserved, including the blames.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Game.h"

#define DISCIPLINES { STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
            STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
            STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
            STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
            STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS }
#define DICE_VALUES {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}
#define END_PATH '\0'

// to be used by playTurns() *only* - I was lazy, so I wrote a macro...
#define roll(dice) nextTurn(g, &whoseTurn, &turnNum, dice)

static void checkStart(Game g);
static void playTurns(Game g);
static void nextTurn(Game g, int *whoseTurn, int *turnNum, int diceValue);
static void printStatus(Game g);

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
    printf("[tg.c] // Gonna catch them all!\n");

    int disciplines[] = DISCIPLINES;
    int diceValues[] = DICE_VALUES;
    Game g = newGame(disciplines, diceValues);
    int id = 0;

    printf("Welcome to the Pokémon Centre.\n\n"
        "Checking the regions are initialised correctly...\n");

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
    int player;

    printf("Checking the initial state...\n");

    assert(getTurnNumber(g) == -1);
    assert(getWhoseTurn(g) == NO_ONE);

    printf("* player stats...\n");
    player = UNI_A;
    while (player <= NUM_UNIS) {
        printf("%d\n", player);

        assert(getARCs(g, player) == 0);
        assert(getGO8s(g, player) == 0);
        assert(getCampuses(g, player) == 2);
        assert(getIPs(g, player) == 0);
        assert(getPublications(g, player) == 0);
        checkStudents(g, player, 0, 3, 3, 1, 1, 1);

        player++;
    }

    printf("* campuses...\n");
    assert(getCampus(g, "RB") == CAMPUS_A);
    assert(getCampus(g, "RLRLRLRLRLL") == CAMPUS_A);
    assert(getCampus(g, "RRLRL") == CAMPUS_B);
    assert(getCampus(g, "RRLRLLRLRL") == CAMPUS_C);
    assert(getCampus(g, "LRLRL") == CAMPUS_C);
    assert(getCampus(g, "LRLRLRRLRL") == CAMPUS_B);

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
    roll(2);
    checkStudents(g, UNI_A, 0, 0, 0, 0, 0, 2);
    obtainArc(g, "LRLRLRRLRL");
    obtainArc(g, "LRLRLRRLR");
    checkStudents(g, UNI_B, 0, 2, 1, 1, 1, 1);
    buildCampus(g, "LRLRLRRL");
    checkStudents(g, UNI_B, 0, 1, 0, 0, 0, 1);

    // 2 UNI_C
    roll(5);
    checkStudents(g, UNI_B, 0, 2, 0, 0, 0, 1);
    obtainArc(g, "RRLRLLRLRLL");
    obtainArc(g, "RRLRLLRLRLLL");
    checkStudents(g, UNI_C, 0, 1, 1, 1, 1, 1);
    buildCampus(g, "RRLRLLRLRLLL");
    checkStudents(g, UNI_C, 0, 0, 0, 0, 0, 1);

    // 3 UNI_A
    roll(4);
    checkStudents(g, UNI_A, 0, 0, 1, 0, 0, 2);

    // 4 UNI_B
    roll(5);
    checkStudents(g, UNI_B, 0, 3, 0, 0, 0, 1);
    checkStudents(g, UNI_C, 0, 1, 0, 0, 0, 1);

    // 5 UNI_C
    roll(9);
    //checkStudents(g, UNI_C, 0, 3, 1, 0, 0, 1);

    // 6 UNI_A
    roll(7);
    checkStudents(g, UNI_A, 2, 0, 1, 0, 0, 0);
    checkStudents(g, UNI_B, 1, 3, 1, 0, 0, 0);
    checkStudents(g, UNI_C, 1, 1, 0, 0, 0, 0);

    // 7 UNI_B
    roll(8);
    //checkStudents(g, UNI_C, 1, 1, 0, 2, 2, 0);

    // 8 UNI_C
    roll(9);
    checkStudents(g, UNI_B, 1, 3, 2, 0, 0, 0);

    // 9 UNI_A
    roll(6);
    //checkStudents(g, UNI_A, 2, 0, 1, 0, 2, 0);

    // 10 UNI_B
    roll(7);
    checkStudents(g, UNI_B, 1, 3, 2, 0, 0, 0);
    //checkStudents(g, UNI_C, 3, 1, 0, 0, 2, 0);
    //checkStudents(g, UNI_A, 2, 0, 1, 0, 2, 0);

    // 11 UNI_C
    roll(9);
    checkStudents(g, UNI_B, 1, 3, 3, 0, 0, 0);

    // 12 UNI_A
    roll(5);
    checkStudents(g, UNI_B, 1, 4, 3, 0, 0, 0);
    //checkStudents(g, UNI_C, 3, 2, 0, 0, 2, 0);

    // 13 UNI_B
    roll(10);
    checkStudents(g, UNI_B, 1, 4, 4, 0, 0, 0);
    assert(getExchangeRate(g, UNI_B, STUDENT_BQN, STUDENT_MMONEY) == 2);
    retrain(g, STUDENT_BQN, STUDENT_MMONEY);
    checkStudents(g, UNI_B, 1, 4, 2, 0, 0, 1);
    assert(getExchangeRate(g, UNI_B, STUDENT_BPS, STUDENT_MMONEY) == 3);
    retrain(g, STUDENT_BPS, STUDENT_MMONEY);
    checkStudents(g, UNI_B, 1, 1, 2, 0, 0, 2);

    // 14 UNI_C
    roll(11);
    //checkStudents(g, UNI_A, 2, 0, 1, 1, 2, 0);

    // 15 UNI_A
    roll(8);

    /* Matt */

    // 16 UNI_B
    roll(9);
    checkStudents(g, UNI_B, 1, 1, 3, 0, 0, 2);
    //obtainArc(g, "LLRLRLLLL");
    //checkStudents(g, UNI_B, 1, 0, 2, 0, 0, 2);

    // 17 UNI_C
    roll(4);
    // 18 UNI_A
    roll(7);
    // 19 UNI_B
    roll(7);
    // 20 UNI_C
    roll(5);
    // 21 UNI_A
    roll(12);
    // 22 UNI_B
    roll(7);
    // 23 UNI_C
    roll(6);
    // 24 UNI_A
    roll(7);
    // 25 UNI_B
    roll(7);
    // 26 UNI_C
    roll(9);

    // 27 UNI_A
    roll(11);

    // 28 UNI_B
    roll(7);
    // 29 UNI_C
    roll(6);
    // 30 UNI_A
    roll(10);

    /* David */
    // XXX Matt hasn't finished his turns yet

    // 31 UNI_B
    roll(7);
    obtainArc(g, "LRLRRLRLL");

    // 32 UNI_C
    roll(9);

    // 33 UNI_A
    roll(6);
    retrain(g, STUDENT_MJ, STUDENT_BPS);
    obtainArc(g, "LRRLRLRLR");

    // 34 UNI_B
    roll(9);
    retrain(g, STUDENT_BQN, STUDENT_MTV);
    retrain(g, STUDENT_BQN, STUDENT_MJ);
    buildCampus(g, "LRRLRLLR");

    // 35 UNI_C
    roll(7);
    // 36 UNI_A
    roll(7);
    // 37 UNI_B
    roll(6);
    // 38 UNI_C
    roll(9);
    // 39 UNI_A
    roll(4);
    // 40 UNI_B
    roll(3);

    // 41 UNI_C
    roll(5);
    retrain(g, STUDENT_MJ, STUDENT_MMONEY);
    startSpinoff(g, TRUE);

    // 42 UNI_A - the meaning of life
    roll(9);

    // 43 UNI_B
    roll(11);
    obtainArc(g, "LRRLRLL");
    obtainArc(g, "LRRLRLLR");

    // 44 UNI_C
    roll(10);
    retrain(g, STUDENT_BPS, STUDENT_BQN);

    // 45 UNI_A
    roll(7);
    retrain(g, STUDENT_MJ, STUDENT_BPS);
    obtainArc(g, "LRRLRLRLR");

    printStatus(g);

    /* Dominic */
    // XXX Matt hasn't finished his turns yet

    /* WIP
    // 46 UNI_B
    roll(10);
    retrain(g, STUDENT_BQN, STUDENT_BPS);
    retrain(g, STUDENT_BQN, STUDENT_MTV);
    buildCampus(g, ""); // @32

    // 47 UNI_C
    roll(4);

    // 48 UNI_A
    roll(12);
    
    // 49 UNI_B
    roll(6);
    
    // 50 UNI_C
    roll(5);
    obtainArc(g, ""); // @33
    
    // 51 UNI_A
    roll(10);

    // 52 UNI_B
    roll(9);
    obtainArc(g, ""); // @47
    obtainArc(g, ""); // @32
    
    // 53 UNI_C
    roll(9);

    // 54 UNI_A
    roll(6);
    retrain(g, STUDENT_MJ, STUDENT_BPS); // 3
    obtainArc(g, ""); // @52

    // 55 UNI_B
    roll(6);

    // 56 UNI_C
    roll(11);
    
    // 57 UNI_A
    roll(10);

    // 58 UNI_B
    roll(9);
    retrain(g, STUDENT_BQN, STUDENT_BPS); // 2
    buildCampus(g, ""); // @22
    retrain(g, STUDENT_BQN, STUDENT_MTV); // 2
    retrain(g, STUDENT_MJ, STUDENT_MMONEY); // 3
    startSpinoff(g, FALSE);

    // 59 UNI_C
    roll(3);
    
    // 60 UNI_A
    roll(6);
    */
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
    int player = getWhoseTurn(g);
    int numCampuses = getCampuses(g, player);
    int kpiPoints = getKPIpoints(g, player);

    printf("  * build campus: %s\n", location);

    assert(getCampus(g, location) == VACANT_VERTEX);
    gameAction.actionCode = BUILD_CAMPUS;
    strncpy(gameAction.destination, location, PATH_LIMIT - 1);
    gameAction.destination[PATH_LIMIT - 1] = END_PATH;

    assert(isLegalAction(g, gameAction));
    makeAction(g, gameAction);

    assert(getCampuses(g, player) == numCampuses + 1);
    assert(getCampus(g, location) == player);
    assert(getKPIpoints(g, player) == kpiPoints + 10);
}

static void buildGO8(Game g, path location) {
    action gameAction;
    int player = getWhoseTurn(g);
    int numNormalCampuses = getCampuses(g, player);
    int numGO8 = getGO8s(g, player);
    int kpiPoints = getKPIpoints(g, player);

    printf("  * build GO8: %s\n", location);

    assert(getCampus(g, location) == player);

    gameAction.actionCode = BUILD_GO8;
    strncpy(gameAction.destination, location, PATH_LIMIT - 1);
    gameAction.destination[PATH_LIMIT - 1] = END_PATH;

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
    int player = getWhoseTurn(g);
    int numARCs = getARCs(g, player);

    printf("  * obtain ARC: %s\n", location);

    assert(getARC(g, location) == NO_ONE);

    gameAction.actionCode = OBTAIN_ARC;
    strncpy(gameAction.destination, location, PATH_LIMIT - 1);
    gameAction.destination[PATH_LIMIT - 1] = END_PATH;

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
    int rate = getExchangeRate(g, player, disciplineFrom, disciplineTo);

    printf("  * retrain: %d => %d\n"
        "    current students: %d, %d\n"
        "    rate: %d\n",
        disciplineFrom, disciplineTo, fromCount, toCount, rate);

    assert(fromCount >= rate);

    action gameAction;
    gameAction.actionCode = RETRAIN_STUDENTS;
    gameAction.disciplineFrom = disciplineFrom;
    gameAction.disciplineTo = disciplineTo;

    assert(isLegalAction(g, gameAction));
    makeAction(g, gameAction);

    assert(getStudents(g, player, disciplineFrom) == fromCount - rate);
    assert(getStudents(g, player, disciplineTo) == toCount + 1);
}

static void checkStudents(
        Game g, int player,
        int countThD, int countBPS, int countBQn,
        int countMJ, int countMTV, int countMMoney) {
    printf("  students: %d: %dxThD %dxBPS %dxB? %dxMJ %dxMTV %dxM$\n",
        player, countThD, countBPS, countBQn, countMJ, countMTV, countMMoney);

    assert(getStudents(g, player, STUDENT_THD) == countThD);
    assert(getStudents(g, player, STUDENT_BPS) == countBPS);
    assert(getStudents(g, player, STUDENT_BQN) == countBQn);
    assert(getStudents(g, player, STUDENT_MJ) == countMJ);
    assert(getStudents(g, player, STUDENT_MTV) == countMTV);
    assert(getStudents(g, player, STUDENT_MMONEY) == countMMoney);
}

static void printStatus(Game g) {
    int player = UNI_A;
    while (player <= NUM_UNIS) {
        printf("player %d:\n"
            "%dxThD %dxBPS %dxB? %dxMJ %dxMTV %dxM$\n",
            player,
            getStudents(g, player, STUDENT_THD),
            getStudents(g, player, STUDENT_BPS),
            getStudents(g, player, STUDENT_BQN),
            getStudents(g, player, STUDENT_MJ),
            getStudents(g, player, STUDENT_MTV),
            getStudents(g, player, STUDENT_MMONEY));
        printf("%d ARCs %d campuses %d GO8s\n",
            getARCs(g, player),
            getCampuses(g, player),
            getGO8s(g, player));
        printf("%d IP %d Pubs\n", 
            getIPs(g, player), getPublications(g, player));
        player++;
    }
}
