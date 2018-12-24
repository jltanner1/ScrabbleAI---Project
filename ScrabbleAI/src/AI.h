#ifndef AI_H_
#define AI_H_

#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "Player.h"
#include "Board.h"
#include "Trie.h"
#include "Move.h"

class Board;

class Trie;

class Move;

class ParseMove;

class Bag;

class Dictionary;

class AI : public Player {

	public:
		AI(std::string const & name, size_t maxTiles, bool isAI, bool setType);
		virtual ~AI();	
		bool getType();

		std::set<std::pair<size_t,size_t>> findStartsEmpty(Board* _board, bool dir); //finds starting squares when board is empty
		std::set<std::pair<size_t,size_t>> findStarts(Board* _board, bool dir); //finds starting squares when board is not empty

		void tryMoves(Board* _board, size_t y, size_t x, TrieNode* root, std::string toPlace, std::string toPush, std::set<Tile*> hand, std::vector<std::string> &words, bool dir, bool connected);
		std::pair<Move*,int> filter(Board* _board, std::vector<std::pair<std::vector<std::string>, std::pair<size_t,size_t>>> vertWords, 
			std::vector<std::pair<std::vector<std::string>, std::pair<size_t,size_t>>> horWords, Bag* _bag, Dictionary* _dict, Player* _p);

	protected:
		bool LetterCPU; //true if CPU plays for max letters, false if CPU plays for max score
};

#endif /* AI_H_ */