#include "Move.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include "Board.h"

using namespace std;

Move::Move(Player * player) { 
	_player = player;
}

Move::~Move() { }

pair<Move*,int> Move::parseAIMoves(Board* _board, TrieSet* _trie, Player* p, Bag* _bag, Dictionary* _dict) {

	AI* current_AI = dynamic_cast<AI*>(p);

	set<pair<size_t,size_t>> startingVerticalIndices; //vertical starting squares
	set<pair<size_t,size_t>> startingHorizontalIndices;	//horizontal starting squares

	if (_board->getStart()->isOccupied() == false && _board->initFileUsed() == false) { //if board is empty
		startingVerticalIndices = current_AI->findStartsEmpty(_board, true);	//vertical search for starting squares
		startingHorizontalIndices = current_AI->findStartsEmpty(_board,false); //horizontal search for starting squares
	}
	else {
		startingVerticalIndices = current_AI->findStarts(_board, true);	//vertical search for starting squares
		startingHorizontalIndices = current_AI->findStarts(_board,false); //horizontal search for starting squares
	}
    
	set<pair<size_t,size_t>>::iterator it;

	vector<pair<vector<string>,pair<size_t,size_t>>> verticalWords;	//all legal vertical moves and starting locations
	vector<pair<vector<string>,pair<size_t,size_t>>> horizontalWords; //all legal horizontal moves and starting locations

	set<Tile*> curHand = current_AI->getHandTiles(); //AI's current hand

	for (it = startingVerticalIndices.begin(); it != startingVerticalIndices.end(); ++it) {

		vector<string> verticalWordCombos;
		current_AI->tryMoves(_board, it->first, it->second, _trie->prefix(""), "", "", curHand, verticalWordCombos, true, false); //find vertical words

		if(!(verticalWordCombos.empty()))verticalWords.push_back(make_pair(verticalWordCombos, *it));
	}
    
	for (it = startingHorizontalIndices.begin(); it != startingHorizontalIndices.end(); ++it) {

		vector<string> horizontalWordCombos;
		current_AI->tryMoves(_board, it->first, it->second, _trie->prefix(""), "", "", curHand, horizontalWordCombos, false, false); //find horizontal words
		
		if(!(horizontalWordCombos.empty())) horizontalWords.push_back(make_pair(horizontalWordCombos, *it));
	}
    
	pair<Move*,int> toMove = current_AI->filter(_board, verticalWords, horizontalWords, _bag, _dict, p); //find the optimal move
    
	return toMove;
}

Move* Move::parseMove (string moveString, Player &p) {

	for (unsigned int i = 0; i < moveString.size(); ++i) {
		moveString[i] = tolower(moveString[i]);	//make everything lowercase
	}

	Move* m = nullptr;
	stringstream ss(moveString);
	string word;
	char direction;
	int start_x, start_y;	//starting positions for word
	string letters;		//string to place as word
	bool horizontal = false;	//true for horizontal word, false for vertical

	ss >> word;
	
	if (word == "pass") {	//if chosen move is a pass

		m = new PassMove(&p);
	}
	else if (word == "exchange") {	//if chosen move is exchange
		string letters;

		ss >> letters;

		m = new ExchangeMove(letters, &p);
	}
	else if (word == "place") {	//if chosen move is place
		ss >> direction >> start_y >> start_x >> letters;

		if (direction == '-') {
			horizontal = true;
		}
		else if (direction == '|') {
			horizontal = false;
		}

		m = new PlaceMove(start_y, start_x, horizontal, letters, &p);
	}

	return m;
}

PassMove::PassMove (Player * player) : Move(player) {
	_player = player;
}

pair<int,int> PassMove::execute(Board & board, Bag & bag, Dictionary & dictionary) {
	return make_pair(0,0);
}

ExchangeMove::ExchangeMove(string tileString, Player * p) : Move(p) {
	_player = p;
	input = tileString;
}

pair<int,int> ExchangeMove::execute(Board & board, Bag & bag, Dictionary & dictionary) {
	bool have_letters = _player->hasTiles(input,false);

	vector<Tile*> drawn;
	vector<Tile*> taken;

	if (have_letters == true && bag.tilesRemaining() >= input.size()) {

		taken = _player->takeTiles(input, false);	//take tiles to exchange
		drawn = bag.drawTiles((size_t)taken.size());	//draw new tiles
		_player->addTiles(drawn);						//add new tiles to player hand
		bag.addTiles(taken);					//put taken tiles back into bag
	}
	else if (have_letters == true && bag.tilesRemaining() < input.size()) {
		throw NotEnoughTilesInBag("Error: not enough remaining tiles to exchange");
	}
	else {
		throw dontHaveTiles("Error: don't have necessary tiles");
	}

	return make_pair(0,0);
}

PlaceMove::PlaceMove(size_t y, size_t x, bool horizontal, string tileString, Player * p) : Move(p) {
	x_start = x;
	y_start = y;
	dir = horizontal;
	input = tileString;
	_player = p;


	if(_player->isAI()) {
		place_vec = _player->takeTiles(input,true); //we know AI's have the necessary tiles
	}
	
	else if (_player->hasTiles(input,true)) {	//if players has the requested 

		place_vec = _player->takeTiles(input,true);
	}
}

vector<Tile*> const & PlaceMove::tileVector () const {
	return place_vec;

}

pair<int,int> PlaceMove::execute(Board & board, Bag & bag, Dictionary & dictionary) {

	int sum = 0;
	bool invalid_word = false;
	vector<pair<string,unsigned int >> possible_words;
	possible_words = board.getPlaceMoveResults(*this);
	
	if (possible_words.size() == 0) {	//no pairs added to the vector
		addBack();
		clearVec();
		throw noValidWordPossibilites("Error: No valid word detected with PlaceWord");
	 }

	Square* start = board.getStart();

	string wordToPlace = "";	//the actual word being placed down --> to check for adjacency 
	for (unsigned int i = 0; i < tileVector().size(); i++) wordToPlace += tileVector()[i]->getUse();

	if ((possible_words.size() == 1) && (get<0>(possible_words[0]) == wordToPlace) && (start->isOccupied() || board.initFileUsed())) {
		addBack();	//no adjacent letters detected for words other than the first
		clearVec();
		throw noAdjacentLettersDetected("Error: placed word does not touch other letters");
	}

	for (unsigned int i = 0; i < possible_words.size(); i++) {	//check for legal words
		if ((dictionary.isLegalWord(get<0>(possible_words[i]))) == false) {
			invalid_word = true;
		}
		sum += get<1>(possible_words[i]);
	}

	if (invalid_word == true) {	//if illegal word is detected
		addBack();
		clearVec();
		throw invalidWordException("Error: Invalid words detected");

	}

	if (place_vec.size() == _player->getMaxTiles()) {	// if player used all tiles in their hand for this move
		sum += 50;		//bonus 50 points
	}

	int numletters = input.size();

	for (unsigned int i = 0; i < input.size(); ++i) {	//to account for ?'s
		if(input[i] == '?') --numletters; //decrease # of letters used by 1 for each ?
	}

	if (_player->isAI()) {	//don't execute until optimal move is found
		addBack();
		clearVec();
		return make_pair(sum, numletters);
	}
	else {

		board.executePlaceMove(*this);	//place tiles on the board
		drawAndScore(sum, *this, bag);
		return make_pair(0,0);
	}
}

void PlaceMove::drawAndScore(int _sum, PlaceMove &m, Bag & bag) {

	vector<Tile*> drawn; //player drawing tiles

	_player->addScore(_sum);	//add the player's successful move points to their score
    
	if (bag.tilesRemaining() >= place_vec.size()) {
		drawn = bag.drawTiles((size_t)place_vec.size());	//draw new tiles
		_player->addTiles(drawn);	//add tiles to player hand
		clearVec();
	}
	else if (bag.tilesRemaining() < place_vec.size() && bag.tilesRemaining() > 0){
		drawn = bag.drawTiles(bag.tilesRemaining());	//draw new tiles
		_player->addTiles(drawn);	//add tiles to player hand
		clearVec();
	}
	else {
        clearVec();
		throw bagEmpty("Bag is now empty");	//bag is empty
	}
}

string PlaceMove::getString() const {
	return input;
}

size_t PlaceMove::getY() const {	//return the y-cooridnates of place
	return y_start;
}

size_t PlaceMove::getX() const {	//return the x-coordinates of place
	return x_start;
}

bool PlaceMove::getDir() const {	//return whether wors is horizontal or vertical
	return dir;
}
size_t PlaceMove::getSize() const {	//return size of input
	return (size_t)input.size();
}

Player* PlaceMove::getP() const {	//return player 
	return _player;
}

void PlaceMove::clearVec() {

	if (place_vec.empty()) return;

	while (place_vec.empty() == false) {	//clear the vector
		place_vec.pop_back();
	}
}

void PlaceMove::addBack() {

	if (place_vec.empty() == false) {	//add tiles back to hand when a false move is detected
		_player->addTiles(place_vec);
	}
}

void PlaceMove::fillVec(string input) {		//add tiles into the vector of tiles to place
	place_vec = _player->takeTiles(input,true);
}
