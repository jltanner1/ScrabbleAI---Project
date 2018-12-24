#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "AI.h"


AI::AI(std::string const & name, size_t maxTiles, bool isAI, bool setType) : Player(name, maxTiles, isAI), LetterCPU(setType) { }

AI::~AI() { }


bool AI::getType() {
	return LetterCPU;
}

std::set<std::pair<size_t,size_t>> AI::findStartsEmpty(Board* _board, bool dir) {

	std::set<std::pair<size_t,size_t>> temp;

	size_t hSize = this->getHandTiles().size();

	size_t startY = _board->getStartRow();	//starting row
	size_t startX = _board->getStartCol();	//starting column

	for (size_t i = 0; i < hSize; ++i) {

		if(dir) {
			if (startY-i > 0) temp.insert(std::make_pair(startY-i,startX)); //adding above start for vertical words
		}

		else {
			if (startX-i > 0) temp.insert(std::make_pair(startY, startX-i)); //adding left of start for horizontal words
		}
	}
	return temp;
}

std::set<std::pair<size_t,size_t>> AI::findStarts(Board* _board, bool dir) {

	std::set<std::pair<size_t,size_t>> temp;

	size_t hSize = this->getHandTiles().size(); //handsize of player
	size_t counter = 0;	

	bool AddedThisIteration = false; 	//flag if square to place was found on a given iteration
	bool foundSpaceBelow = false;		//flags for squares to place found above and below occupied squares
	bool foundSpaceAbove = false;

	for (size_t i = 1; i <= _board->getColumns(); ++i) {

		for (size_t j = _board->getRows(); j > 0; --j) {

			AddedThisIteration = false;	//reset bool flags
			foundSpaceBelow = false;
			foundSpaceAbove = false;
			counter = 0;				//reset counter for vertical/horiozontal searches below

			if (_board->getSquare(i,j)->isOccupied()) {	// hit an occupied square
				
				if (j+1 <= _board->getRows() && _board->getSquare(i,j+1)->isOccupied() == false) {	//search below

					temp.insert(std::make_pair(j+1,i));
					AddedThisIteration = true;
					foundSpaceBelow = true;
				}

				if (j-1 > 0 && _board->getSquare(i,j-1)->isOccupied() == false) {	//search above

					temp.insert(std::make_pair(j-1,i));
					AddedThisIteration = true;
					foundSpaceAbove = true;
				}
				counter = hSize-1;
			}

			else {
				if (i != 1) {	//looking left
			
					if ((_board->getSquare(i,j)->isOccupied() == false) && _board->getSquare(i-1,j)->isOccupied()) {

						temp.insert(std::make_pair(j,i));
						AddedThisIteration = true;
					}
				}

				if (i != _board->getColumns()) {	//looking right
					
					if ((_board->getSquare(i,j)->isOccupied() == false) && _board->getSquare(i+1,j)->isOccupied()) {

						temp.insert(std::make_pair(j,i));
						AddedThisIteration = true;
					}
				}
				counter = hSize-1;
			}

			if (AddedThisIteration && dir) {

				size_t i_copy = i;
				size_t j_copy = j-1;	//one above just added

				if(foundSpaceAbove) --j_copy;	//for the above case

				while (counter > 0 && j_copy > 0) {

					if (_board->getSquare(i_copy,j_copy)->isOccupied() == false) {
						temp.insert(std::make_pair(j_copy,i_copy));
					}
					--j_copy;
					--counter;
				}
			}

			else if (AddedThisIteration && !dir) {

				size_t i_copy = i-1;	//left space
				size_t j_copy = j;

				if(!foundSpaceBelow && !foundSpaceAbove) {	//regular left checks

					while (counter > 0 && i_copy > 0) {

						if((_board->getSquare(i_copy,j_copy)->isOccupied() == false)) {
							temp.insert(std::make_pair(j_copy,i_copy));
						}
						--i_copy;
						--counter;
					}
				}

				while (foundSpaceBelow || foundSpaceAbove) {

					if (foundSpaceBelow) j_copy = j+1;	//space below 
					else j_copy = j-1;	//space above

					while (counter > 0 && i_copy > 0 && (j_copy > 0 && j_copy <= _board->getRows()) ) {

						if (_board->getSquare(i_copy,j_copy)->isOccupied() == false) {
							temp.insert(std::make_pair(j_copy,i_copy));
						}
						--i_copy;
						--counter;
					}

					i_copy = i-1;	//reset i_copy in case while loop runs again

					if (foundSpaceBelow) { 
						foundSpaceBelow = false;	//after searching square added below
						counter = hSize-1;
					}
					else foundSpaceAbove = false;	//after searching square added above
				}
			}	
		}
	}
	return temp;
}

void AI::tryMoves(Board* _board, size_t y, size_t x, TrieNode* root, std::string toPlace, std::string toPush, std::set<Tile*> hand, std::vector<std::string> &words, bool dir, bool connected) {

	if ((int)y > (int)_board->getRows() || (int)x > (int)_board->getColumns() || (int)x < 0 || (int)y < 0) return;	//base case out of bounds check
	if (hand.size() == 0) { //base case if you run out of tiles
        
		//to add occupied squares after all tiles in hand are used
        bool incorrect = false;
        
		if (_board->getSquare(x,y)->isOccupied() && root->children[std::tolower(_board->getSquare(x,y)->getLetter())-'a'] != nullptr) {
            
			if (dir == true)
				while (y < _board->getRows()) {	//vertical word
                                        
					if (_board->getSquare(x,y)->isOccupied() && root->children[std::tolower(_board->getSquare(x,y)->getLetter())-'a'] != NULL) {
						toPlace = toPlace + _board->getSquare(x,y)->getLetter();
						root = root->children[std::tolower(_board->getSquare(x,y)->getLetter())-'a'];
						y++;
					}
                    else {
                        incorrect = true;
                        break;
                    }
				}
			else {	//horizontal word
				while (x < _board->getColumns()) {
					if (_board->getSquare(x,y)->isOccupied() && root->children[std::tolower(_board->getSquare(x,y)->getLetter())-'a'] != NULL) {
						toPlace = toPlace + _board->getSquare(x,y)->getLetter();
						root = root->children[std::tolower(_board->getSquare(x,y)->getLetter())-'a'];
						x++;
					}
                    else {
                        incorrect = true;
                        break;
                    }
				}
			}
		}
        
		if (root->inSet == true && connected == true && incorrect == false) words.push_back(toPush); //if prefix is a valid word
		return;	
	}

	std::set<Tile*>::iterator it;			//used to iterate through AI hand
	std::set<Tile*> tempHand = hand;		//copy of AI hand
	std::set<Tile*> remainingHand = hand;	//amended hand to pass recursively
	bool tempConnected = connected;			//if a word being traversed is connected to other words on board

	std::string temp = toPlace;		//string to check as a valid word for this level of recursion
	std::string tempPush = toPush;	//string of valid hand Tiles for move
	TrieNode* current = root;		//current location in Trie

	if (_board->getSquare(x,y)->isOccupied()) {		//if board is occupied for these cooridnates

		char board_c = _board->getSquare(x,y)->getLetter();
		temp = temp + (board_c);
		tempConnected = true;

		if(current->children[std::tolower(board_c) - 'a'] != NULL) { 	//if the word exists

				current = current->children[std::tolower(board_c) - 'a'];
				if (dir == true) tryMoves(_board, y+1, x, current, temp, tempPush, remainingHand, words, dir, tempConnected); //recurse down for vertical words
				else tryMoves(_board, y, x+1, current, temp, tempPush, remainingHand, words, dir, tempConnected);	//recurse right for horizontal words

			if (current->inSet == true && tempConnected == true) {	//if prefix is a valid word
				words.push_back(tempPush); //so only tiles are in the string
			}
		}
		return;
	}

	else {
        
		if(dir == true && toPlace.size() == 0) { //look above for occupied squares on first recursive call
			size_t y_copy = y;

			while (y_copy-1 > 0) {
				if (_board->getSquare(x,y_copy-1)->isOccupied()) {
					temp = _board->getSquare(x,y_copy-1)->getLetter() + temp;
					y_copy = y_copy-1;
					tempConnected = true;
				}
				else break;
			}
		}

		else if (dir == false && toPlace.size() == 0){ 	//look left for occupied squares on first recursive call
			size_t x_copy = x;
			while (x_copy-1 > 0) {
				if (_board->getSquare(x_copy-1,y)->isOccupied()) {
					temp = _board->getSquare(x_copy-1,y)->getLetter() + temp;
					x_copy = x_copy-1;
					tempConnected = true;
				}
				else break;
			} 
		}
        
		//connection to occupied squares checks
		if (_board->initFileUsed() == false && _board->getStart()->isOccupied() == false 
			&& _board->getStartRow() == y && _board->getStartCol() == x) tempConnected = true; //empty board case hitting the starting square

		if (y+1 <= _board->getRows()) { // check down for vertical and horizontal words
			if (_board->getSquare(x,y+1)->isOccupied() == true) tempConnected = true;
		}

		if (x+1 <= _board->getColumns()) { //check right for vertical and horizontal words
			if (_board->getSquare(x+1,y)->isOccupied() == true) tempConnected = true;
		}

		if (dir == true && x-1 > 0) {	//vertical connection check left
			if(_board->getSquare(x-1,y)->isOccupied() == true) tempConnected = true;
		}

		else if (dir == false && y-1 > 0) { //horizontal check up
			if (_board->getSquare(x,y-1)->isOccupied() == true) tempConnected = true;
		}
        
		//iterate through the players hand at this level of recusrion
		for (it = tempHand.begin(); it != tempHand.end(); ++it) {
            
			if ((*it)->getLetter() == '?') { //if current letter in hand is a '?'
				char temp_letter;

				for (int i = 0; i < 26; ++i) { //test all possible letters that '?' could be
					temp_letter = 'a' + i;
					temp += temp_letter; //add a letter

					if (current->children[std::tolower(temp_letter) - 'a'] != NULL) { //only check further if word exists thus far in trie

						current = current->children[std::tolower(temp_letter) - 'a']; //update node in trie
						tempPush += '?'; //add a question mark
						tempPush += temp_letter; //add a letter

						remainingHand.erase(*it); //delete the '?' from hand

						if (dir == true) tryMoves(_board, y+1, x, current, temp, tempPush, remainingHand, words, dir, tempConnected); //recurse down for vertical words
						else tryMoves(_board, y, x+1, current, temp, tempPush, remainingHand, words, dir, tempConnected);	//recurse right for horizontal words
						
						if  (current->inSet == true && tempConnected == true) words.push_back(tempPush); //if prefix is a valid word

						current = root;		//reset TrieNode* for next iteration
						tempPush = tempPush.substr(0,tempPush.size()-2); //to delete '?' and letter
						remainingHand.insert(*it); //to add '?' back into the hand
					}

					temp = temp.substr(0,temp.size()-1); //to delete the letter for other possible word combos
				}

			}

			else {	//if letter current letter in hand is not a '?'

				char letter = (*it)->getLetter();
				temp = temp + letter;
				tempPush = tempPush + letter;

				if (current->children[std::tolower(letter) - 'a'] != NULL) { //only check further if word exists thus far in trie
					current = current->children[std::tolower(letter) - 'a']; //update node in trie
					remainingHand.erase(*it); //delete letter from hand

					if (dir == true) tryMoves(_board, y+1, x, current, temp, tempPush, remainingHand, words, dir, tempConnected); //recurse down for vertical words
					else tryMoves(_board, y, x+1, current, temp, tempPush, remainingHand, words, dir, tempConnected);	//recurse right for horizontal words

					if (current->inSet == true && tempConnected == true) {
						words.push_back(tempPush); //if prefix is a valid word
					}

					current = root; //reset TrieNode* for next iteration
					remainingHand.insert(*it); //to add letter back into hand
				}
				temp = temp.substr(0,temp.size()-1); //delete letter
				tempPush = tempPush.substr(0,tempPush.size()-1); //delete letter
			}	
		}
		return;
	}
}

std::pair<Move*,int> AI::filter(Board* _board, std::vector<std::pair<std::vector<std::string>, std::pair<size_t,size_t>>> vertWords, 
	std::vector<std::pair<std::vector<std::string>, std::pair<size_t,size_t>>> horWords, Bag* _bag, Dictionary* _dict, Player* _p) {

	std::pair<Move*,int> result = std::make_pair(nullptr,-1); //pair to save optimal move and score of that move
	Move* curr = NULL;
	std::pair<int,int> scoreAndLetters; //pair of score and size of each move

	int maxScore = -1;		// max score of all moves
	int maxLetters = -1;	//max letters used by all moves

	AI* current_AI = dynamic_cast<AI*>(_p);

	for (unsigned int i = 0; i < vertWords.size(); ++i ) {	//loop through each pair of possible vertical strings at certain indices

		for (unsigned int j = 0; j < vertWords[i].first.size(); ++j) {	//loop through all possible strings at index ^^^
			try {
				curr = new PlaceMove((int)vertWords[i].second.first, (int)vertWords[i].second.second, false, vertWords[i].first[j], _p);
				scoreAndLetters = curr->execute((*_board), (*_bag), (*_dict)); //get score and size of current move
			}
			catch (NotEnoughTilesInBag& e) {
				delete curr;
				continue;
			}
			catch (dontHaveTiles& e) {
				delete curr;
				continue;
			}
			catch (notTouchingStartingSquareException& e){
				delete curr;
				continue;
			}
			catch (positionOutOfBounds& e) {
				delete curr;
				continue;
			}
			catch (firstSpaceAlreadyOccupied& e) {
				delete curr;
				continue;
			}
			catch(noValidWordPossibilites& e) {
				delete curr;
				continue;
			}
			catch(noAdjacentLettersDetected& e) {
				delete curr;
				continue;
			}
			catch(invalidWordException& e) {
				delete curr;
				continue;
			}

			if (scoreAndLetters.first > maxScore && current_AI->getType() == false) { //CPUS check
				maxScore = scoreAndLetters.first;

				if (result.first != nullptr) delete result.first; //delete previous optimal move
				result = std::make_pair(curr, scoreAndLetters.first); //save new optimal move
			}
			else if (scoreAndLetters.second > maxLetters && current_AI->getType() == true) { //CPUL check
				maxLetters = scoreAndLetters.second;

				if (result.first != nullptr) delete result.first;	//delete previous optimal move
				result = std::make_pair(curr, scoreAndLetters.first); //save new optimal move
			}
		}
	}

	for (unsigned int i = 0; i < horWords.size(); ++i) { //loop through each pair of possible horizontal strings at certain indices

		for (unsigned int j = 0; j < horWords[i].first.size(); ++j) { //loop through all possible strings at index ^^^
			
			try {
				curr = new PlaceMove((int)horWords[i].second.first, (int)horWords[i].second.second, true, horWords[i].first[j], _p);
				scoreAndLetters = curr->execute((*_board), (*_bag), (*_dict)); //get score and size of current move
			}
			catch (NotEnoughTilesInBag& e) {
				delete curr;
				continue;
			}
			catch (dontHaveTiles& e) {	
				delete curr;
				continue;
			}
			catch (notTouchingStartingSquareException& e){
				delete curr;
				continue;
			}
			catch (positionOutOfBounds& e) {
				delete curr;
				continue;
			}
			catch (firstSpaceAlreadyOccupied& e) {
				delete curr;
				continue;
			}
			catch(noValidWordPossibilites& e) {
				delete curr;
				continue;
			}
			catch(noAdjacentLettersDetected& e) {
				delete curr;
				continue;
			}
			catch(invalidWordException& e) {
				delete curr;
				continue;
			}
			
			if (scoreAndLetters.first > maxScore && current_AI->getType() == false) { //CPUS check
				maxScore = scoreAndLetters.first;

				if (result.first != nullptr) delete result.first; //delete previous optimal move
				result = std::make_pair(curr, scoreAndLetters.first); //save new optimal move
			}
			else if (scoreAndLetters.second > maxLetters && current_AI->getType() == true) { //CPUL check
				maxLetters = scoreAndLetters.second;

				if (result.first != nullptr) delete result.first;	//delete previous optimal move
				result = std::make_pair(curr, scoreAndLetters.first); //save new optimal move
			}
		}
	}

	if (result.first == nullptr) { //if no legal moves, AI defaults to Pass
		curr = new PassMove(_p);
		return std::make_pair(curr, 0);
	}

	return result; //return optimal AI move
}
