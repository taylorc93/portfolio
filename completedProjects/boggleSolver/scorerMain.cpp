#include <iostream>
#include <cstdlib>
#include "BogScorer.h"
using namespace std;

int main(){
	BogScorer scorer;
	scorer.readWords();
	scorer.outputResults();
	return 0;
}
