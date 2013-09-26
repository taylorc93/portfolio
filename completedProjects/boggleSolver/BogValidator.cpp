#include <iostream>
#include <cstdlib>
#include "BogSolver.h"
#include "BogValidator.h"
using namespace std;

//Constructor, initializes the wordList
BogValidator::BogValidator(){
	wordList = new BogWordList;
}

//Destructor, destroys the wordList
BogValidator::~BogValidator(){
	for (int i = 0; i < wordList->numWords; i++){
		delete [] wordList->words[i].letts;
	}
//	delete words->words;
	delete wordList;
}

//calls the solver's readDict function to read in the dictionary, returns true
//if there are no errors
bool BogValidator::readDict(){
	if (!solver.readDict()){
		return false;
	}
	return true;
}

//calls the solver's readBoard function to get the board, returns true if there
//are no errors
bool BogValidator::readBoard(){
	if (!solver.readBoard()){
		return false;
	}
	return true;
}

//Reads in all the users words.  Checks each one to see if they are valid:
//(len >= 3, on the board, and isnt a duplicate).  If its good, it stores it
//in the validUserWords, if not, in the invalidUserWords.  Then, outputs all
//the words in each vector
void BogValidator::checkWords(){
	string s;
	solver.solve();

	while (cin >> s){
		s = cleanString(s);
		if (isValid(s)){
			validUserWords.push_back(s);
		}
		else{
			invalidUserWords.push_back(s);
		}
	}

	for (int i = 0; i < validUserWords.size(); i++){
		cout << "OK " << validUserWords[i] << endl;;
	}
	for (int i = 0; i < invalidUserWords.size(); i++){
		cout << "NO " << invalidUserWords[i] << endl;
	}
}

//Checks if a specific word is valid by calling the 2 private functions and
//testing its length. Returns true if the word is valid
bool BogValidator::isValid(string s){
	if (alreadyAdded(s)){
		return false;
	}
	else if (notOnBoard(s)){
		return false;
	}
	else if (s.length() < 3){
		return false;
	}
	else{
		return true;
	}
}

//Makes sure that all letters in the string are uppercase
string BogValidator::cleanString(string s){
	for (int i = 0; i < s.length(); i++){
		s[i] = toupper(s[i]);
	}
	return s;
}

//Goes through the validUserWords array and tests to see if the string is 
//already there.  Returns true if it has already been added to validUserWords
bool BogValidator::alreadyAdded(string s){
	for (int i = 0; i < validUserWords.size(); i++){
		if (s == validUserWords[i]){
			return true;
		}
	}
	return false;
}

//Checks to see if a word is not on the board. Gets all the words found on the
//board that are the same length as s and tests to see if s is equal to any of
//them.  Returns true if the word is not found on the board.
bool BogValidator::notOnBoard(string s){
	int len = s.length();
   	wordList = solver.getWords();

	for (int i = 0; i < wordList->numWords; i++){
		string word = bogToString(wordList->words[i]);
	   	if (word == s){
			return false;
		}
	}
//	for (int i = 0; i < wordList->numWords; i++){
//		delete [] wordList->words[i].letts;
//	}
//	delete words->words;
//	delete words;

	return true;
}

//Converts a BogWord to a string
string BogValidator::bogToString(BogWord w){
    string temp = "";
    for (int i = 0; i < w.numLetts; i++){
        temp += w.letts[i].c;
    }
    return temp;
}

