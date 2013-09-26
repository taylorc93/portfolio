#include <iostream>
#include <cstdlib>
#include <cctype>
#include "dictionary.h"
using namespace std;

void readInWords(Dictionary dict);

int main(){

	Dictionary dict;
	readInWords(dict);
	dict.isWord("omneity");
}

void readInWords(Dictionary dict){
	string s;

	while (cin >> s){
		dict.insert(s);
	}
}
