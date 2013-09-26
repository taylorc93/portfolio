#include <iostream>
#include <cstdlib>
#include <cctype>
#include "BogValidator.h"
using namespace std;

int main(){
	BogValidator v;
	v.readDict();
	v.readBoard();
	v.checkWords();
	return 0;
}
