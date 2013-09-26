#ifndef dictionary
#define dictionary

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

//
// This is the header file for the Dictionary and TrieNode classes for project
// 2.  The TrieNode class has a bool stating whether that node represents a
// real word and an array of pointers to TrieNodes (there are 26 to represent
// each letter of the alphabet). The Dictionary class has 3 public functions:
// one that tests if a string is a prefix, one that tests if a string is a
// word, and one that inserts a new word into the dictionary. There are also
// 2 private functions: one that removes any character that isnt alphabetical
// from a string and one that converts a char to an int.
//
// NOTE: I'm not 100% I used two tokens on the last project, but I am using 
// one for this part.  I submitted token2.txt, but its possible it should have
// been token1.txt
//
const int NUM_LETTERS = 26;

class TrieNode{

public:

	TrieNode();
    bool      realWord;
	TrieNode* nextLetter[NUM_LETTERS];
};

class Dictionary{

public:

	Dictionary();
	bool isPrefix(string s);
	bool isWord(string s);

	//returns false if there is no more space for any words
	bool insert(string s);

private:

	//removes any characters that are not alphabetical from a string
	string 		cleanWord(string input);
	//converts a char to an int (a is 0, b is 1, etc)
	int    		charToInt(char c);
	TrieNode* 	root;	
};

#endif

