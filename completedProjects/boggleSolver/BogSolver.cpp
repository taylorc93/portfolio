#include <iostream>
#include <cctype>
#include <cstdlib>
#include "BogSolver.h"
using namespace std;

//Constructor, initializes all pointer data members
BogSolver::BogSolver(){
	board = NULL;
	wordList = new BogWordList;
	wordList->numWords = 0;
	wordList->words = new BogWord[INIT_SIZE_WORDS];
	wordList->cap = INIT_SIZE_WORDS;
}

//Reads in the dictionary until it hits the sentinel value "."
bool BogSolver::readDict(){
	string	s;

	while (true){
		cin >> s;
		if (s == "."){
			break;
		}
		if(!(dict.insert(s))){
			return false;
	  	}
	}
	return true;
}

//Reads in the Boggle Board.  Stores the height and width in the BogSolver 
//object. After initializing the letters of the board, the cells get linked to
//their adjacent cells. Returns true if there were no errors in doing any of
//these things
bool BogSolver::readBoard(){
	string	s;
	BogLett	letter;
	bool	success;

	cin >> height >> width;
	success = makeBoard(height, width);
	if (!success){
		return false;
	}
	for (int i = 0; i < height; i++){
		if (cin >> s){
			if (s.length() != width){
				return false;
	   		}
			for (int k = 0; k < width; k++){
			   	letter.c	= toupper(s[k]);
				letter.row	= i;
				letter.col	= k;
   				board[i][k].lett		= letter;
				board[i][k].adjacents	= NULL; 
				board[i][k].beenUsed	= false;
			}
		}
		else{
			return false;
		}
	}
	linkAdjacents(height, width);
	return true;
}

//Dynamically allocates the memory necessary for making the board. Returns false
//if their isnt enough memory
bool BogSolver::makeBoard(int dim1, int dim2){
    board = new BogCell* [dim1];
    if (board != NULL){
        for (int i = 0; i < dim1; i++){
            board[i] = new BogCell[dim2];
            if (board[i] == NULL){
                return false;
            }
        }
    }
    else{
        return false;
    }
	return true;
}

//Loops through the board and initialized the adjacents pointer for each 
//BogCell, then links them.
void BogSolver::linkAdjacents(int dim1, int dim2){
	for (int i = 0; i < dim1; i++){
		for (int k = 0; k < dim2; k++){
			board[i][k].adjacents = new BogCell* [NUM_ADJACENT];
			board[i][k].numAdj = 0;
			makeLink(i, k, dim1, dim2);
		}
	}
}

//Loops through a 3x3 square area with the cell at position row,col in the
//center.  Tests to make sure that the current place of i and k isnt out of
//bounds or equal to row and col, and then links the two cells.
void BogSolver::makeLink(int row, int col, int dim1, int dim2){
	int pos = 0;
	for (int i = row - 1; i <= row + 1; i++){
		for (int k = col - 1; k <= col + 1; k++){
			if ((i == row) && (k == col)){
				continue;
			}
			else if ((((i < 0)||(i == dim1)) || ((k == dim2)||(k < 0)))){
				continue;
			}
			else{
	   			board[row][col].adjacents[pos] = &board[i][k];
				pos++;
				board[row][col].numAdj++;
			}
		}	
	}
}

//Public solver.  Loops through the board and creates a new BogWord to start
//with for the recursive solve function (tests for QU).  Passes the address
//of the BogCell, the bogword, and the length of the word to the recursive
//function and returns true if nothing goes wrong.
bool BogSolver::solve(){
	for (int i = 0; i < height; i++){
		for (int k = 0; k < width; k++){
			BogWord* w = new BogWord;
			if (board[i][k].lett.c != 'Q'){
				w->numLetts = 1;
				w->letts = new BogLett[1];
				w->letts[0] = board[i][k].lett;
			}
			else{
				w->numLetts = 2;
				w->letts = new BogLett[2];
				w->letts[0] = board[i][k].lett;
				w->letts[1].c = 'U';
				w->letts[1].row = -1;
				w->letts[1].col = -1;
			}
	 	  	solve(&board[i][k], w, w->numLetts);
//			delete [] w->letts;
//			delete w;
		}
	}
	return true;
}

//Recursive solve function.  Gets the string of the BogWord and sets the 
//beenUsed property to true to prevent double-counting.  Tests if the string
//is a prefix (meaning their are words that can be made from it).  If yes,
//it tests if its a valid word to insert (len >= 3) and inserts it.  Then,
//it recurses through all of the adjacent cells.  If the word is not a prefix,
//it returns.  After all recursion is done, deletes dynamically allocated mem
//and resets beenUsed. Returns true if nothing goes wrong.
bool BogSolver::solve(BogCell* root, BogWord* w, int len){
	string s = bogToString(*w);
    root->beenUsed = true;

	if (dict.isPrefix(s)){
		if ((len >= 3) && (dict.isWord(s))){
			insertWord(*w);
		}
		for (int i = 0; i < root->numAdj; i++){
    	    if (!root->adjacents[i]->beenUsed){
				BogWord* temp = expandWord(root->adjacents[i]->lett, w, len);
				solve(root->adjacents[i], temp, temp->numLetts);			
				delete [] temp->letts;
				delete temp;
			}
		}
	}

	root->beenUsed = false;		
   	return true;
}

//Converts a bogword to a string
string BogSolver::bogToString(BogWord w){
	string temp = "";
	for (int i = 0; i < w.numLetts; i++){
		temp += w.letts[i].c;
	}
	return temp;
}

//Adds the adjacent cells letter to the bogword in the recursive solve function.
//Tests for QU and returns the new, expanded word. Deletes the old word.
BogWord* BogSolver::expandWord(BogLett adj, BogWord* w, int len){
	BogWord* temp = new BogWord;
   	if (adj.c == 'Q'){
   		temp->numLetts = len + 2;
   	    temp->letts = new BogLett [len + 2];
   	}
   	else{
   		temp->numLetts = len + 1;
   		temp->letts = new BogLett [len + 1];
   	}
   	for (int k = 0; k < len; k++){
   		temp->letts[k] = w->letts[k];
   	}

	if (adj.c == 'Q'){
		temp->letts[len] = adj;
		temp->letts[len +1].c = 'U';
        temp->letts[len +1].row = -1;
        temp->letts[len +1].col = -1;
	}
	else{
		temp->letts[len] = adj;
	}

//	delete [] w->letts;
//	delete w;
	return temp;
}

//Inserts a word into the wordList.  Returns if the word is already found in
//the dictionary.  Places the word in the wordList and expands it if need be.
void BogSolver::insertWord(BogWord w){
	string s = bogToString(w);
	int pos = wordList->numWords;

	for (int i = 0; i < pos; i++){
		string temp = bogToString(wordList->words[i]);
		if (temp == s){
			return;
		}
	}
	if (pos == wordList->cap){
		wordList = expandList(wordList);
	}

	wordList->words[pos].letts = new BogLett [w.numLetts];
	wordList->words[pos].numLetts = w.numLetts;
	for (int i = 0; i < w.numLetts; i++){
		wordList->words[pos].letts[i] = w.letts[i];
	}
	wordList->numWords++;
}

//Expands the BogWordList.  Creates a new BogWordList, copies all the data over
//to it, and deletes the old one. Returns the new wordList.
BogWordList* BogSolver::expandList(BogWordList* w){
	BogWordList* temp = new BogWordList;
    temp->cap = w->numWords * 2;
	temp->words = new BogWord [temp->cap];
	temp->numWords = w->numWords;

	for (int i = 0; i < temp->numWords; i++){
		temp->words[i].numLetts = w->words[i].numLetts;
		temp->words[i].letts = new BogLett [temp->words[i].numLetts];
		for (int k = 0; k < temp->words[i].numLetts; k++){
			temp->words[i].letts[k] = w->words[i].letts[k];
		 }
	}
    for (int i = 0; i < w->numWords; i++){
		delete [] w->words[i].letts;
	}
	delete [] w->words;
	delete w;

	return temp;
}

//Returns the number of words in wordList
int BogSolver::numWords(){
	return wordList->numWords;
}

//Returns the number of words of size len in wordList
int BogSolver::numWords(int len){
	int num = 0;
	for (int i = 0; i < wordList->numWords; i++){
		if (wordList->words[i].numLetts == len){
			num++;
		}
	}
	return num;
}

//Deep copies the word list and returns it so that a user does not have access 
//to private data.  Returns the copied wordList
BogWordList* BogSolver::getWords(){
	BogWordList* temp = new BogWordList;
	temp->words = new BogWord [wordList->numWords];
	temp->numWords = wordList->numWords;

	for (int i = 0; i < wordList->numWords; i++){
		temp->words[i].numLetts = wordList->words[i].numLetts;
		temp->words[i].letts = new BogLett [temp->words[i].numLetts];
		for (int k = 0; k < temp->words[i].numLetts; k++){
			temp->words[i].letts[k] = wordList->words[i].letts[k];
		}
	}
	return temp;
}

//Makes a deep copy of all words of length len in wordList and returns it.
BogWordList* BogSolver::getWords(int len){
    BogWordList* temp = new BogWordList;
    temp->words = new BogWord [wordList->numWords];
    temp->numWords = 0;
	int pos = 0;

    for (int i = 0; i < wordList->numWords; i++){
		cout << wordList->words[i].numLetts << endl;
		if (wordList->words[i].numLetts == len){
			cout << "test passed" << endl;
        	temp->words[pos].numLetts = wordList->words[i].numLetts;
    		temp->words[pos].letts = new BogLett [temp->words[pos].numLetts];
    	   	for (int k = 0;	k < temp->words[i].numLetts; k++){
       		    temp->words[pos].letts[k] = wordList->words[i].letts[k];
//				cout << temp->words[pos].letts[k].c;
   			}
//			cout << endl;
	   		temp->numWords++;
			pos++;
    	}
	}
    return temp;
}

//Prints all words in the wordList in HBF (with the row and col location of each
//letter printed after it.  
void BogSolver::printWords(){
	for (int i = 0; i < wordList->numWords; i++){
		cout << "< ";
		for (int k = 0; k < wordList->words[i].numLetts; k++){
			cout << wordList->words[i].letts[k].c;
			if (wordList->words[i].letts[k].c == 'Q'){
				cout << 'U';
			}
			cout << " " << wordList->words[i].letts[k].row << " "
				 << wordList->words[i].letts[k].col << " "; 
			if (wordList->words[i].letts[k].c == 'Q'){
				k++;
			} 
		}
		cout << ">" << endl;
	}
}

//Prints all words of length len in the wordList in HBF
void BogSolver::printWords(int len){
    for	(int i = 0; i <	wordList->numWords; i++){
		if (wordList->words[i].numLetts == len){
	    	cout <<	"< ";
	        for (int k = 0; k < wordList->words[i].numLetts; k++){
				cout << wordList->words[i].letts[k].c;
				if (wordList->words[i].letts[k].c == 'Q'){
					cout << 'U';
				}
				cout << " " << wordList->words[i].letts[k].row << " "
					 << wordList->words[i].letts[k].col << " ";
				if (wordList->words[i].letts[k].c == 'Q'){
                    k++;
				}
			}
		}
    	cout <<	">" << endl;
	}
}

//Outputs all words in the wordList as simple strings
void BogSolver::listWords(){
    for (int i = 0; i < wordList->numWords; i++){
		string s = bogToString (wordList->words[i]);
		cout << s << endl;
    }
}

//Outputs all words of length len in the wordList as simple strings
void BogSolver::listWords(int len){
	for (int i = 0; i < wordList->numWords; i++){
        string s = bogToString (wordList->words[i]);
     	if (s.length() == len){
		   cout << s << endl;
		}
    }
}

//Destructor, frees all allocated space associated with the private data members
//of the BogSolver
BogSolver::~BogSolver(){
	for (int i = 0; i < height; i++){
		for (int k = 0; k < width; k++){
			delete [] board[i][k].adjacents;
		}
		delete [] board[i];
	}
	delete [] board;

	for (int i = 0; i < wordList->numWords; i++){
   		delete [] wordList->words[i].letts;
	}
	delete wordList;
}
