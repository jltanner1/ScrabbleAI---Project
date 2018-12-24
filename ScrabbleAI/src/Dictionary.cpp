#include <set>
#include <string>
#include "Dictionary.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Dictionary::Dictionary (string dictionary_file_name) {

	ifstream infile(dictionary_file_name.c_str());

	if (infile.fail()){
		return;	//throw an exception
	}

	string word;
	string new_word;
	while (getline(infile, word)) {
		CheckCaps(word);	//check for uppercase
		stringstream ss(word);	//check for white space after word
		ss >> new_word;

		dict.insert(new_word);
	}

	infile.close();
}

Dictionary::~Dictionary () {}

bool Dictionary::isLegalWord (string const &word) const {
	set<string>::iterator start;
	
	start = dict.find(word);

	if (start == dict.end()){	//word not found
		return false;
	}
	return true;
}

void Dictionary::CheckCaps(string &temp) {

	for (unsigned int i = 0; i < temp.length(); ++i) {

		temp[i] = tolower(temp[i]);	//change every letter to lowercase
	}
}

set<string> Dictionary::getDict() {
	return dict; 
}