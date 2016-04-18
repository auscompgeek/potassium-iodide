potassium-iodide
===============

Team Hypnotoad's code for the COMP1917 project ("Knowledge Island").

## Building
It is recommended to use `make` to compile this project.

### make targets
* run: build `runGame` and run it.
* test: build `testGame` and run it (default).
* test-all: build `test*` and run them.
* clean: remove the compiled executables.

### make options
* `GAME=otherGameImpl.o`: build `runGame` and `testGame` against a different Game implementation.
* `TESTGAME=testGame.c`: link `$(GAME)` with a different `testGame.c`.

### examples
```
$ make  # build and run testGame
$ make GAME=otanGame.o test  # build and run testGame against otanGame.o (hi @Cyberbully)
```

## Collaboration links (OpenLearning)
* [Game.c](https://www.openlearning.com/courses/99luftballons/Cohorts/ClassOf2014/Groups/ShravanMatthewDavidDominic/Structgame)
* [testGame.c](https://www.openlearning.com/courses/99luftballons/Cohorts/ClassOf2014/Groups/ShravanMatthewDavidDominic/TestgameCCollaboration)
* [Game simulation](https://www.openlearning.com/courses/99luftballons/Cohorts/ClassOf2014/Groups/ShravanMatthewDavidDominic/GamesSimulated/Game1) (littered with errors though)

## Thank you
Kudos to @ismeta and @Cyberbully for running the testing system for the project!

Thank you to @icedtrees for tutoring the UNSW tute, and John, Rafi, Shanush, Kelvin, and Curtis for mentoring! (Wow, that's a lot of mentors.)

Also thank you to @tetrakai for making HS1917 possible this year!

Thank you to @he-lium for helping manage our project so well, @MattSolo for his game simulation, and also to @shravanjeevan.
