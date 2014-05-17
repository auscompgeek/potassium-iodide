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

// Checks students for entire uni
void checkStudents( Game g, int player,
    int countThD, int countBPS, int countBQn,
    int countMJ, int countMTV, int countMMoney);

int main() {
    int disciplines[] = DISCIPLINES;
    int diceValues[] = DICE_VALUES;
    action gameAction;
    Game g = newGame(disciplines, diceValues);

    int id = 0;
    while (id < NUM_REGIONS) {
        assert(getDiscipline(g, id) == disciplines[id]);
        assert(getDiceValue(g, id) == diceValues[id]);
    }
    assert(getTurnNumber(g) == -1);
    assert(getWhoseTurn(g) == NO_ONE);
    assert(getTurnNumber(g) == -1);

    assert(getCampus(g, "RB") == CAMPUS_A);
    assert(getCampus(g, "RLRLRLRLRLL") == CAMPUS_A);
    assert(getCampus(g, "RRLRL" == CAMPUS_B));
    assert(getCampus(g, "RRLRLLRLRL" == CAMPUS_C));
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

    // Insert turns here

    // Dispose game
    disposeGame(g);
    printf("All Tests Passed\n");
    return EXIT_SUCCESS;
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