#include <iostream>
#include <cstdlib>
#include <cctype>
#include <string>
#include "dictionary.h"
using namespace std;

//constructor for the TrieNode class, this sets the realWord bool to false and
//sets all pointers in the nextLetter array to NULL
TrieNode::TrieNode(){
	realWord = false;
	for (int i = 0; i < NUM_LETTERS; i++){
		nextLetter[i] = NULL;
	}
}

//initializes root
Dictionary::Dictionary(){
	root = new TrieNode;
}

//tests if a string is a prefix.  Cleans the word, gets its length, then loops
//through the trie.  In the loop, if nextLetter is not NULL, it continues, 
//otherwise, it returns false.  If it reaches the end of the loop, then the
//string must be a prefix and the function returns true
bool Dictionary::isPrefix(string s){
	string	  word	= cleanWord(s);
	int 	  len	= word.length();
	TrieNode* iter	= root;
    int       index;

	for (int i = 0; i < len; i++){
		index = charToInt(word[i]);
		if (iter->nextLetter[index] != NULL){
			iter = iter->nextLetter[index];
		}
		else{
			return false;
		}	
	}
	return true;
}

//Tests to see if a string is a word.  Does everything that isPrefix does, but
//then additionally tests to see if the realWord bool is true.  If so, then the
//function returns true, otherwise, it returns false.
bool Dictionary::isWord(string s){
    string    word  = cleanWord(s);
    int       len   = word.length();
    TrieNode* iter  = root;
    int       index;

    for (int i = 0; i < len; i++){
        index = charToInt(word[i]);
        if (iter->nextLetter[index] != NULL){
            iter = iter->nextLetter[index];
        }
        else{
            return false;
        }
    }

	if (iter->realWord){
    	return true;
	}
	else{
		return false;
	}
}

//Inserts a new string into the Dictionary.  Cleans the word, gets its length, 
//and then loops through the trie to insert the word in the proper place. If
//the nextLetter pointer is NULL, a new TrieNode is created.  When iter is in
//the proper place, the TrieNode bool is changed to true.  If there is no space
//for a new word, the function returns false, otherwise, it returns true.
bool Dictionary::insert(string s){
	string 		word	= cleanWord(s);
	int    		len    	= word.length();
	TrieNode* 	iter	= root;
	int 		index;

	//test to see if the empty string has been inserted
	if (word == ""){
		root->realWord = true;
		return true;
	}

	for (int i = 0; i < len; i++){
		index	= charToInt(word[i]);
		if (iter->nextLetter[index] == NULL){
			iter->nextLetter[index] = new TrieNode;
			if (iter->nextLetter[index] == NULL){  //tests if there is no more
				return false;					   //memory available
			}
		}
		iter = iter->nextLetter[index];
	}	
	iter->realWord = true;
	return true;
}

int Dictionary::charToInt(char c){
	return (c - 'a');
}

//Cleans a word by removing any non-alphabetic characters.  Creates a new
//string and only inserts the letters that are alphabetic in the other string.
//Returns the clean string.
string Dictionary::cleanWord(string s){
	int		len    	= s.length();
	string	cleanWord;
	char	letter;

	for (int i = 0; i < len; i++){
		if (isalpha(s[i])){
			letter = toupper(s[i]);
		 	cleanWord += letter;
		}
	}
	return cleanWord;
}
