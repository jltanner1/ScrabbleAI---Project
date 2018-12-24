#include <fstream>
#include <sstream>
#include <iostream>
#include "Board.h"

using namespace std;

Board::Board (string board_file_namey) {
	
	using_init_file = false;	//set initialization file flag to false

	ifstream infile(board_file_namey.c_str());

	infile >> x >> y;

	board = new Square**[y];

	for (unsigned int i = 0; i < y; ++i) {	//dynamically allocate a 2D array of Square pointers

		board[i] = new Square*[x];
	}

	infile >> sx >> sy;
	string line;
	getline(infile, line);	//to get rid of white space

	for (unsigned int j = 0; j < y; ++j) {
		
		getline (infile, line);
		stringstream ss(line);
		char space;
		bool play_start = false;

		for (unsigned int i = 0; i < x; ++i) {
			
			ss >> space;

			play_start = false;	//reset flag

			if (j == sy-1 && i == sx-1) {		//if we hit the starting square
				play_start = true;
			}
			
			if (space == '.') {	// normal square 
				Square* temp = new Square(1,1,play_start);
				board[j][i] = temp;
			}

			else if (space == '2' || space == '3') {	//letter multiplier

				if (space == '2') {
					Square* temp = new Square(2,1,play_start);
					board[j][i] = temp;
				}
				else {
					Square* temp = new Square(3,1,play_start);
					board[j][i] = temp;
				}
			}

			else if (space == 'd' || space == 't') {	//word multiplier

				if(space == 'd') {
					Square* temp = new Square(1,2,play_start);
					board[j][i] = temp;
				}
				else {
					Square* temp = new Square(1,3,play_start);
					board[j][i] = temp;
				}
			}
		}
	}
	infile.close();
}

Board::~Board() {	//Deallocate Board and Squares
	
	for (unsigned int j = 0; j < y; ++j) {

		for (unsigned int i = 0; i < x; ++i) {
			
			delete board[j][i];
		}
		delete [] board[j];
	}
	delete [] board;
}

vector<pair<string, unsigned int>> Board::getPlaceMoveResults(const PlaceMove &m) const {
	
	vector< pair<string,unsigned int >> results;	//the return vector
	size_t j = (m.getY()-1);	//row index for PlaceMove
	size_t i = (m.getX()-1);	//column index for PlaceMove
	vector<Tile*> place = m.tileVector();	//accessing place_vec private member variable in Move
	size_t count = place.size();	//size of PlaceMove string
	int index = 0;	//to iterate through place vector
	PlaceMove m_temp = m;

	if ((place).empty()) {
		throw dontHaveTiles("Error: don't have necessary tiles");
	}

	if (board[sy-1][sx-1]->isOccupied() == false && using_init_file == false) {	//check for first play on starting square
																				//don't need to check if init_file was used
		if (m.getDir() == true) {
			if (i > sx-1 || (i+count-1 < (sx-1)) || j != sy-1) {
				m_temp.addBack();
		 		m_temp.clearVec();
				throw notTouchingStartingSquareException("Error: not touching the starting square");
			}
		}

		else {
			if (j > sy-1 || (j+count-1 < (sy-1)) || i != sx-1) {
				m_temp.addBack();
		 		m_temp.clearVec();
				throw notTouchingStartingSquareException("Error: not touching the starting square");
			}
		}
	}

	if (board[j][i]->isOccupied()) {
		m_temp.addBack();
		m_temp.clearVec();
		throw firstSpaceAlreadyOccupied("Error: entered starting position that is already occupied");
	}

	if (m.getDir() == true) {

		if (i < 0 || i+count > getColumns() || j < 0 || j > getRows()) { //bounds check horizontal
			m_temp.addBack();
			m_temp.clearVec();
			throw positionOutOfBounds("Error: entered starting position where word goes out of bounds");
		}

		while (count != 0) {
		
			if(board[j][i]->isOccupied() == false) {
				pair<string,unsigned int> temp_pair = search(place[index],i,j,true);	//search vertically
				if (get<0>(temp_pair).size() > 1) {

					results.push_back(temp_pair);
				}
				++i;
				--count;
				++index;
			}
			else {
				++i;
			}
		}
	}

	else {

		if (j < 0 || j+count > getRows() || i < 0 || i > getColumns()) { //bounds check vertical
			m_temp.addBack();
			m_temp.clearVec();
			throw positionOutOfBounds("Error: entered starting position where word goes out of bounds");
		}
		
		while (count != 0) {

			if (board[j][i]->isOccupied() == false) {
				pair<string,unsigned int> temp_pair = search(place[index],i,j,false);	//search horizontally
				if (get<0>(temp_pair).size() > 1) {

					results.push_back(temp_pair);
				}
				++j;
				--count;
				++index;
			}
			else {
				++j;
			}
		}
	}
	size_t b = (m.getY()-1);
	size_t a = (m.getX()-1);
	pair<string,unsigned int> formed;
	if (m.getDir() == true) {
		formed = original_search(place, a, b, true);
	}
	else {
		formed = original_search(place, a, b, false);
	}
	results.push_back(formed);	//add the final word formed
	return results;
}

void Board::executePlaceMove (const PlaceMove &m) {
	vector<Tile*> place;
	
	place = m.tileVector();
	int index = 0;
	size_t j = (m.getY()-1);
	size_t i = (m.getX()-1);
	size_t count = place.size();

	if (m.getDir() == true) {

		while (count != 0) {
			if ((board[j][i])->isOccupied() == false) {	//if Square is unoccupied

				(board[j][i])->placeTile(place[index]); 
				++index;
				++i;	
				--count;
			}
			else {	//if Square is already occupied
				++i;
			}
		}
	}

	else {
		
		while(count != 0) {

			if (board[j][i]->isOccupied() == false) {	//if Square is unocupied

				(board[j][i])->placeTile(place[index]);
				++index;
				++j;
				--count;
			}
			else {		//if Square is occupied
				++j;
			}
		}
	}
}

Square * Board::getSquare (size_t x, size_t y) const {

	return board[y-1][x-1];
}

size_t Board::getRows() const {

	return (size_t)y;
}

size_t Board::getColumns() const {

	return (size_t)x;
}

Square* Board::getStart() const {
	return board[sy-1][sx-1];
}

pair<string,unsigned int> Board::search(Tile* current, size_t x_pos, size_t y_pos, bool direction) const {

	pair<string,unsigned int> new_pair;
	string temp = "";
	unsigned int score = 0;
	unsigned int word_mult = 1;
	size_t x_copy = x_pos;
	size_t y_copy = y_pos;

	if (direction == true) {	//vertical search for other words when PlaceMove is a horizontal word
		while (y_pos > 0 && (board[y_pos-1][x_pos]->isOccupied() == true)) {
			--y_pos;		//to get y_pos to first space of vertical letters
		}

		while (y_pos < getRows() && ((board[y_pos][x_pos]->isOccupied() == true) || y_pos == y_copy)) {

			if (x_pos == x_copy && y_pos == y_copy) {
				temp += tolower(current->getUse());
				score += (current->getPoints()*board[y_pos][x_pos]->getLMult());
			}	
			else {
				temp += tolower(board[y_pos][x_pos]->getLetter());
				score += (board[y_pos][x_pos]->getScore()*board[y_pos][x_pos]->getLMult());
			}
			word_mult *= board[y_pos][x_pos]->getWMult();
			++y_pos;
		}
		score = score*word_mult;
		new_pair = make_pair(temp,score);
	}

	else {	//horizontal search for other words when PlaceMove is a vertial word
		while(x_pos > 0 && (board[y_pos][x_pos-1])->isOccupied() == true) {
			--x_pos;	//to get x_pos to first space of horizontal letters
		}

		while (x_pos < getColumns() && ((board[y_pos][x_pos]->isOccupied()) || x_pos == x_copy)) {
			
			if (x_pos == x_copy && y_pos == y_copy) {
				temp += tolower(current->getUse());
				score += (current->getPoints()*board[y_pos][x_pos]->getLMult());
			}
			else {
				temp += tolower(board[y_pos][x_pos]->getLetter());
				score += (board[y_pos][x_pos]->getScore()*board[y_pos][x_pos]->getLMult());
			}
			word_mult *= board[y_pos][x_pos]->getWMult();
			++x_pos;

		}
		score = score*word_mult;
		new_pair = make_pair(temp,score);
	}
	return new_pair;
}

pair<string,unsigned int> Board::original_search(vector<Tile*> original, size_t x_pos, size_t y_pos, bool direction) const {
	
	pair<string,unsigned int> new_pair;
	string temp = "";
	unsigned int score = 0;
	unsigned int word_mult = 1;
	size_t x_copy = x_pos;
	size_t y_copy = y_pos;
	size_t string_size = original.size();

	if (direction == true) {	//horizontal search extension of word being placed
		while(x_pos > 0 && (board[y_pos][x_pos-1])->isOccupied() == true) {
			--x_pos;	//to get x_pos to first space of horizontal letters
		}

		while (x_pos < x_copy && (board[y_pos][x_pos]->isOccupied())) {	//letters before word

			temp += tolower(board[y_pos][x_pos]->getLetter());	
			score += (board[y_pos][x_pos]->getScore()*board[y_pos][x_pos]->getLMult());
			word_mult *= board[y_pos][x_pos]->getWMult();
			++x_pos;
		}

		while (x_pos >= x_copy && x_pos < x_copy+string_size) {	//letters in actual word
			while(board[y_pos][x_pos]->isOccupied()) {	//letters between the actual word
				temp += tolower(board[y_pos][x_pos]->getLetter());
				score += board[y_pos][x_pos]->getScore()*board[y_pos][x_pos]->getLMult();
				word_mult *= board[y_pos][x_pos]->getWMult();
				++x_pos;
				++x_copy;
			}

			temp += tolower(original[x_pos-x_copy]->getUse());
			score += (original[x_pos-x_copy]->getPoints()*board[y_pos][x_pos]->getLMult());
			word_mult *= board[y_pos][x_pos]->getWMult();
			++x_pos;
		}

		while (x_pos < getColumns() && (board[y_pos][x_pos]->isOccupied())) {	//letters after the actual word 
			temp += tolower(board[y_pos][x_pos]->getLetter());
			score += (board[y_pos][x_pos]->getScore()*board[y_pos][x_pos]->getLMult());
			word_mult *= board[y_pos][x_pos]->getWMult();
			++x_pos;
		}
		
		score = score*word_mult;
		new_pair = make_pair(temp,score);
	}

	else {		//vertical search for extension of word being placed
		while (y_pos > 0 && (board[y_pos-1][x_pos]->isOccupied() == true)) {
			--y_pos;		//to get y_pos to first space of vertical letters
		}

		while (y_pos < y_copy && (board[y_pos][x_pos]->isOccupied() == true)) {	//letters placed before word
			temp += tolower(board[y_pos][x_pos]->getLetter());
			score += (board[y_pos][x_pos]->getScore()*board[y_pos][x_pos]->getLMult());
			word_mult *= board[y_pos][x_pos]->getWMult();
			++y_pos;
		}

		while (y_pos >= y_copy && y_pos < y_copy+string_size) { //letters in actual word
			while(board[y_pos][x_pos]->isOccupied()) {	//letters on board between actual word
				temp += tolower(board[y_pos][x_pos]->getLetter());
				score += board[y_pos][x_pos]->getScore()*board[y_pos][x_pos]->getLMult();
				word_mult *= board[y_pos][x_pos]->getWMult();
				++y_pos;
				++y_copy;
			}

			temp += tolower(original[y_pos-y_copy]->getUse());
			score += (original[y_pos-y_copy]->getPoints()*board[y_pos][x_pos]->getLMult());
			word_mult *= board[y_pos][x_pos]->getWMult();
			++y_pos;
		}	

		while (y_pos < getRows() && (board[y_pos][x_pos]->isOccupied() == true)) {	//letters after placed word
			temp += tolower(board[y_pos][x_pos]->getLetter());
			score += (board[y_pos][x_pos]->getScore()*board[y_pos][x_pos]->getLMult());
			word_mult *= board[y_pos][x_pos]->getWMult();
			++y_pos;
		}
	
		score = score*word_mult;
		new_pair = make_pair(temp,score);
	}
	return new_pair;
}

void Board::init_board(std::string init_file) {	//check whether we need to delete these tiles

	ifstream infile(init_file.c_str());

	if (!infile) {
		cout << "Error: unable to open the initialization file." << endl;
		return;
	}

	using_init_file = true; //set initialization flag to true

	unsigned int i = 0;
	string line;
	char current;
	char value1;
	char value2;
	Tile* temp = NULL;

	for (unsigned int j = 0; j < y; ++j) {

		getline(infile, line);
		stringstream ss(line);
		i = 0;
		while (ss >> current) {

			if (current == '.') {
				ss >> current;
				ss >> current;	//take in two more periods for empty square
				++i;
			}
			else {
				ss >> value1;	//take in the tile tenths place score
				ss >> value2;	//take in the tile ones place score
				temp = new Tile((char)current, ((int)(value1 - '0')*10 + (int)(value2-'0')));
				board[j][i]->placeTile(temp);
				++i;
			}
		}
	}
}

bool Board::initFileUsed() {
	return using_init_file;
}

size_t Board::getStartRow() {
	return size_t(sy);
}

size_t Board::getStartCol() {
	return size_t(sx);
}
