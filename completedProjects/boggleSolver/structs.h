#ifndef structs
#define structs

#include <iostream>
#include <cctype>
using namespace std;

struct BogLett {
	char c;
	int row, col;
};

struct BogWord {
	int numLetts;
	BogLett* letts;
};

struct BogWordList {
	int numWords;
	int cap; //capacity of words array
	BogWord* words;
};

struct BogCell {
	BogLett   lett;
   	BogCell** adjacents;
	int 	  numAdj;
	bool	  beenUsed;
};

#endif

