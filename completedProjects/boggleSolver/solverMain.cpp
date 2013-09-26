#include <iostream>
#include "BogSolver.h"

using namespace std;

int main(){
	BogSolver solver;
	solver.readDict();
	solver.readBoard();
	solver.solve();
	solver.printWords();
	return 0;
}
