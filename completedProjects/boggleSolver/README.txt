bbsolver: g++ -Wall -Wextra Dictionary.cpp BogSolver.cpp solverMain.cpp
bbchecker: g++ -Wall -Wextra Dictionary.cpp BogSolver.cpp BogValidator.cpp 
checkerMain.cpp
bbscorer: g++ -Wall -Wextra BogScorer.cpp scorerMain.cpp

These three programs were written by Connor Taylor for Comp15

These programs will interact with a website to allow a user to play a game of
boggle.
