// Dominic He
// Matthew Solomonson
// David Vo
// Shravan Jeevan
// 09/05/2014
// Tests the Game ADT Interface

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

void checkStart(Game g);
void playTurns(Game g);
void nextTurn(Game g, int *whoseTurn, int *turnNum, int diceValue);

void pass(Game g);
void buildCampus(Game g, path location);
void buildGO8(Game g, path location);
void obtainArc(Game g, path location);
void startSpinoff(Game g, int obtainPatent);
void retrain(Game g, int disciplineFrom, int disciplineTo);

void checkStudents( Game g, int player,
    int countThD, int countBPS, int countBQn,
    int countMJ, int countMTV, int countMMoney);

int main() {
    int disciplines[] = DISCIPLINES;
    int diceValues[] = DICE_VALUES;
    Game g = newGame(disciplines, diceValues);

    int id = 0;
    while (id < NUM_REGIONS) {
        assert(getDiscipline(g, id) == disciplines[id]);
        assert(getDiceValue(g, id) == diceValues[id]);
    }
    checkStart(g);
    playTurns(g);

    // Dispose game
    disposeGame(g);
    printf("All Tests Passed\n");
    return EXIT_SUCCESS;
}

void checkStart(Game g) {
    assert(getTurnNumber(g) == -1);
    assert(getWhoseTurn(g) == NO_ONE);
    assert(getTurnNumber(g) == -1);

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
    checkStudents(g, UNI_A, 0, 3, 3, 1, 1, 1);
    checkStudents(g, UNI_A, 0, 3, 3, 1, 1, 1);
}

void playTurns(Game g) {
    int whoseTurn = NO_ONE;
    int turnNum = -1;

    // 0 UNI_A
    nextTurn(g, &whoseTurn, &turnNum, 5);
    checkStudents(g, UNI_B, 0, 4, 3, 1, 1, 1);
    obtainArc(g, "LRRLRLRLRLR");
    obtainArc(g, "LRRLRLRLRL");
    checkStudents(g, UNI_A, 0, 1, 1, 1, 1, 1);
    buildCampus(g, "LRRLRLRLR");
    // 1 UNI_B
    nextTurn(g, &whoseTurn, &turnNum, 2);
    obtainArc(g, "LRLRLRRLRL");
    obtainArc(g, "LRLRLRRLR");
    buildCampus(g, "LRLRLRRL");
    // 2 UNI_C
    nextTurn(g, &whoseTurn, &turnNum, 5);
    obtainArc(g, "RRLRLLRLRLL");
    obtainArc(g, "RRLRLLRLRLLL"); 
    buildCampus(g, "RRLRLLRLRLLL"); 

    // 3 UNI_A
    nextTurn(g, &whoseTurn, &turnNum, 4); 

    // 4 UNI_B
    nextTurn(g, &whoseTurn, &turnNum, 5);
    
    // 5 UNI_C
    nextTurn(g, &whoseTurn, &turnNum, 9);
    
    // 6 UNI_A
    nextTurn(g, &whoseTurn, &turnNum, 7);
    
    // 7 UNI_B
    nextTurn(g, &whoseTurn, &turnNum, 8);
    
    // 8 UNI_C
    nextTurn(g, &whoseTurn, &turnNum, 9);
    
    // 9 UNI_A
    nextTurn(g, &whoseTurn, &turnNum, 6);
    
    // 10 UNI_B
    nextTurn(g, &whoseTurn, &turnNum, 7);
    
    // 11 UNI_C
    nextTurn(g, &whoseTurn, &turnNum, 9);
    
    // 12 UNI_A
    nextTurn(g, &whoseTurn, &turnNum, 5);
    
    // 13 UNI_B
    nextTurn(g, &whoseTurn, &turnNum, 10);
    assert(getExchangeRate(g, UNI_B, STUDENT_BQN, STUDENT_MMONEY) == 2);
    retrain(g, STUDENT_BQN, STUDENT_MMONEY);
    assert(getExchangeRate(g, UNI_B, STUDENT_BPS, STUDENT_MMONEY) == 3);
    retrain(g, STUDENT_BPS, STUDENT_MMONEY);
    checkStudents(g, UNI_B, 1, 1, 2, 0, 0, 2);

    // 14 UNI_C
    nextTurn(g, &whoseTurn, &turnNum, 11);

    // 15 UNI_A
    nextTurn(g, &whoseTurn, &turnNum, 8);

    nextTurn(g, &whoseTurn, &turnNum, 9);

    // TODO turn 
}

void nextTurn(Game g, int *whoseTurn, int *turnNum, int diceValue) {
    pass(g);
    *whoseTurn = *whoseTurn + 1;
    if (*whoseTurn > UNI_C) {
        *whoseTurn = UNI_A;
    }
    *turnNum = *turnNum + 1;
    throwDice(g, diceValue);
    assert(getWhoseTurn(g) == *whoseTurn);
    assert(getTurnNumber(g) == *turnNum);
}

void pass(Game g) {
    action gameAction;
    gameAction.actionCode = PASS;
    assert(isLegalAction(g, gameAction) == TRUE);
    makeAction(g, gameAction);
}

void buildCampus(Game g, path location) {
    action gameAction;
    int i;
    int numCampuses = getCampuses(g, getWhoseTurn(g));
    int kpiPoints = getKPIpoints(g, getWhoseTurn(g));

    assert(getCampus(g, location) == VACANT_VERTEX);
    gameAction.actionCode = BUILD_CAMPUS;
    i = 0;
    while (location[i] != END_PATH) {
        gameAction.destination[i] = location[i];
        i++;
    }
    gameAction.destination[i] = END_PATH;
    assert(isLegalAction(g, gameAction) == TRUE);
    makeAction(g, gameAction);

    assert(getCampuses(g, getWhoseTurn(g)) == numCampuses + 1);
    assert(getCampus(g, location) == getWhoseTurn(g));
    assert(getKPIpoints(g, getWhoseTurn(g)) == kpiPoints + 10);
}

void buildGO8(Game g, path location) {
    action gameAction;
    int i;
    int numNormalCampuses = getCampuses(g, getWhoseTurn(g));
    int numGO8 = getGO8s(g, getWhoseTurn(g));
    int kpiPoints = getKPIpoints(g, getWhoseTurn(g));

    assert(getCampus(g, location) == getWhoseTurn(g));
    gameAction.actionCode = BUILD_GO8;
    i = 0;
    while (location[i] != END_PATH) {
        gameAction.destination[i] = location[i];
        i++;
    }
    gameAction.destination[i] = END_PATH;
    assert(isLegalAction(g, gameAction) == TRUE);
    makeAction(g, gameAction);

    assert(getCampuses(g, getWhoseTurn(g)) == numNormalCampuses - 1);
    assert(getGO8s(g, getWhoseTurn(g) == numGO8 + 1));
    // GO8 campuse code = normal code + 3
    assert(getCampus(g, location) == getWhoseTurn(g) + 3);
    assert(getKPIpoints(g, getWhoseTurn(g)) == kpiPoints + 10);
}

void obtainArc(Game g, path location) {
    action gameAction;
    int i;
    int numARCs = getARCs(g, getWhoseTurn(g));

    assert(getARC(g, location) == NO_ONE);
    gameAction.actionCode = OBTAIN_ARC;
    i = 0;
    while (location[i] != END_PATH) {
        gameAction.destination[i] = location[i];
        i++;
    }
    gameAction.destination[i] = END_PATH;
    assert(isLegalAction(g, gameAction) == TRUE);
    makeAction(g, gameAction);

    assert(getARC(g, location) == getWhoseTurn(g));
    assert(getARCs(g, getWhoseTurn(g)) == numARCs + 1);
}

void startSpinoff(Game g, int obtainPatent) {
    action gameAction;
    int count;

    gameAction.actionCode = START_SPINOFF;
    assert(isLegalAction(g, gameAction) == TRUE);
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

void retrain(Game g, int disciplineFrom, int disciplineTo) {
    int fromCount = getStudents(g, getWhoseTurn(g), disciplineFrom);
    int toCount = getStudents(g, getWhoseTurn(g), disciplineTo);
    assert(fromCount >= getExchangeRate(g, getWhoseTurn(g), 
        disciplineFrom, disciplineTo));

    action gameAction;
    gameAction.actionCode = RETRAIN_STUDENTS;
    gameAction.disciplineFrom = disciplineFrom;
    gameAction.disciplineTo = disciplineTo;
    assert(isLegalAction(g, gameAction) == TRUE);
    makeAction(g, gameAction);
    assert(getStudents(g, getWhoseTurn(g), disciplineFrom) == 
        fromCount - getExchangeRate(g, getWhoseTurn(g), 
        disciplineFrom, disciplineTo));
    assert(getStudents(g, getWhoseTurn(g), disciplineTo) ==
        toCount + 1);
}

void checkStudents(
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