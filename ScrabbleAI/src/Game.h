//the game class that manages all iterations of the game

//#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <set>
#include <vector>
#include <string>
#include "Bag.h"
#include "Board.h"
#include "Dictionary.h"
#include "Player.h"
#include "Move.h"
#include "Square.h"
#include "Tile.h"
#include "Exceptions.h"
#include "ConsolePrinter.h"
#include "Trie.h"
#include "AI.h"

//class Board;	//forward declarations
//class Player;
//class Bag;

class Game {

public:

	//constructor for the game, given the inputs from the config file
	Game (size_t h_size, std::string t_file, std::string d_file, std::string b_file, uint32_t _s, std::string i_file);

	//destructor for the game
	~Game();

	//to create the players for the game
	std::vector<Player*> makePlayers();

	//to print all player scores to the terminal
	void printScores();

	//the main function of game.  will run until a winner is found
	void runGame();

	//prints the final scores at end of the end of game
	void printFinalScores();

private:
	Board* _board;
	Bag* _bag;
	Dictionary* _dict;
	std::vector<Player*> _player_vec;
	size_t _handsize;
	uint32_t _seed;
	ConsolePrinter p;
	TrieSet* _trie;
};