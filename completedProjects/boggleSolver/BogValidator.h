/*
	BogValidator Program
	Written by: Connor Taylor
	Date: 5/1/13
	This program will read in a dictionary, a boggle board, and a list of user
	words and check if the users words are valid answers and should be given
	points.  The program uses the BogSolver to hold the dictionary and board,
	and also to solve the board to find out if the words are on the board.
*/

#ifndef BOG_V
#define BOG_V

#include <iostream>
#include <cctype>
#include <cstdlib>
#include <vector>
#include "structs.h"
#include "Dictionary.h"
#include "BogSolver.h"
using namespace std;

class BogValidator{

public:
	BogValidator(); // constructor
	~BogValidator(); // destructor

	bool readDict(); // read in a dictionary
	bool readBoard(); // read in a board
	bool isValid(string s); // validates one word

	void checkWords(); // validates cin

private:
	Dictionary dict; // must use a Dictionary
	vector<string> validUserWords; //stores valid words the user inputted
	vector<string> invalidUserWords; //stores bad user words
	vector<string> foundWords; //words the solver found
	BogSolver solver; //solver to check for 
	BogWordList* wordList; //holds the words found in notOnBoard

	bool alreadyAdded(string s); //checks if a string is in validUserWords
	bool notOnBoard(string s); //checks if an inputted word is not on the board

	string bogToString(BogWord w); //converts a BogWord to a string
	string cleanString(string s); //makes sure all the letters are capitalized
};

#endif

