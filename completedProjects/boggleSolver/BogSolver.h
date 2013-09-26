/*
	BogSolver Program	
	Written By: Connor Taylor
	Date: 5/1/13
	This program will read in a dictionary and a boggle board and find all the 
	words that are both on the board and in the dictionary.  The program will
	print the words in HBF after the board has been solved, but can also print
	the words as simple strings.  The program can also return a BogWordList of
	the words that it found and the number of words that it found.  
 */

#ifndef BOG_S
#define BOG_S

#include <iostream>
#include <cstdlib>
#include <cctype>
#include "structs.h"
#include "Dictionary.h"

using namespace std;

const int NUM_ADJACENT = 8;
const int INIT_SIZE_WORDS = 25;
class BogSolver{

//MUST DO
// Note: QU is handled such that the U is placed in a seperate BogLett after Q
// with row and col set to -1.
// 2. Fix mem leaks

public:

	BogSolver();
	~BogSolver();

	bool readDict();
	bool readBoard();
	bool solve(); // search board for words in dict

	int numWords(); // returns number of words found
	int numWords(int len); // number of words of length len

	BogWordList* getWords(); // returns all words found
	BogWordList* getWords(int len); // returns words of len len

    //Potentially add sorting to output
	void printWords(); // print all words in HBF
	void printWords(int len); // print len-length words in HBF
	void listWords(); // print just the text, no coords
	void listWords(int len); // just the text, no coords

private:

	Dictionary dict; // must use a Dictionary
	BogCell** board; //the 2d array representing the board
	BogWordList* wordList; //stores all the words found in BogWordList form

	int 	height; //height of the board
   	int 	width; //width of the board

	bool makeBoard(int dim1, int dim2); //creates the boggle board
	void linkAdjacents(int dim1, int dim2); //links together adjacent cells 
   	void makeLink(int row, int col, int dim1, int dim2); //creates a link

	bool solve(BogCell* root, BogWord* w, int len); //recursive solve function
	string bogToString(BogWord w); //converts a bogword to a string
	BogWord* expandWord(BogLett adj, BogWord* w, int len);
 	void insertWord(BogWord w); //inserts a word into wordList

	BogWordList* expandList(BogWordList* w); //expands a dynamic array
	BogWord	expandWord(BogWord w);
};

#endif

