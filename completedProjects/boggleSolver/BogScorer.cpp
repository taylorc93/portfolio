#include <iostream>
#include <cstdlib>
#include "BogScorer.h"
using namespace std;

//Constructor, sets the number of words and the total score to 0
BogScorer::BogScorer(){
	totalScore = 0;
	numWords = 0;
}

//Reads in the checker report.  If a word is preceded by "OK", it adds it to 
//validWords, otherwise, it goes to invalidWords.  
void BogScorer::readWords(){
	string s;
	while (cin >> s){
		if (s == "OK"){
			cin >> s;
			validWords.push_back(s);
		}
		else{
			cin >> s;
			invalidWords.push_back(s);
		}
	}
}

//Outputs the results.  Assigns a score to each word and adds that to the total
//score. After outputting all words and their individual scores, outputs the 
//total number of words and the total score.
void BogScorer::outputResults(){
	int score;
	for (int i = 0; i < validWords.size(); i++){
		if (validWords[i].length() < 5){
			score = 1;
		}
		else if (validWords[i].length() < 6){
			score = 2;
		}
		else if (validWords[i].length() < 7){
			score = 3;
		}
		else if (validWords[i].length() < 8){
			score = 5;
		}
		else{
			score = 11;
		}
		totalScore += score;
		numWords++;
		cout << score << " OK " << validWords[i] << endl;
	}

	for (int i = 0; i < invalidWords.size(); i++){
		cout << "0 " << "NO " << invalidWords[i] << endl; 
	}

	cout << numWords << " words " << totalScore << " score" << endl;
}

