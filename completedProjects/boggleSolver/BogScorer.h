/*	BogScorer Program
	Written by: Connor Taylor
	Date: 5/1/13
	This prorgram will take in a report that is sent to it from the BogValidator
	and figure out the score that the user should receive.  It outputs the score
	for each word the user sends and the total score along with the total number
	of words found.
 */

#ifndef BOG_SCORER
#define BOG_SCORER

#include <iostream>
#include <cstdlib>
#include <cctype>
#include <vector>
using namespace std;

class BogScorer{

public:

	BogScorer();
	void readWords(); //reads in the checker report and stores the words
	void outputResults(); //outputs the number of words and the score

private:

	vector<string> validWords; //holds the valid words
	vector<string> invalidWords; //holds the invalid words
	int totalScore; //users total score
	int numWords; //users total number of words found
};

#endif

