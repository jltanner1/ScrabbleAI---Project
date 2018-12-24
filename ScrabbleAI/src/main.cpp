#include "Game.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {

	ifstream infile(argv[1]);

	string line;

	string string1;
	string tile_file;
	string board_file;
	string dict_file;
	size_t _handsize;
	uint32_t _seed;
	string init_file = "";
    
	while (getline(infile,line)) {

		stringstream ss(line);
		ss >> string1;

		if (string1 == "HANDSIZE:") {	//read in handsize for each player
			ss >> _handsize;
		}

		else if (string1 == "TILES:") {	//read in the file full of tile points and counters
			ss >> tile_file;
		}

		else if (string1 == "DICTIONARY:") {	//read in all legal words
			ss >> dict_file;
		}

		else if (string1 == "BOARD:") {	//read in the layout of the board
			ss >> board_file;
		}

		else if (string1 == "SEED:"){	//read in seed value for bag
			ss >> _seed; 
		}

		else if(string1 == "INIT:") {	//read in initialization file for initial tile placement
			ss >> init_file;
		}
	}

	infile.close();

	Game* g = new Game(_handsize, tile_file, dict_file, board_file, _seed, init_file);	//create a game

	g->runGame();	//run the game until a winner is found
    
	g->printFinalScores();	//print out result of ended game
        
	delete g;	//delete game pointer

	return 0;
}
