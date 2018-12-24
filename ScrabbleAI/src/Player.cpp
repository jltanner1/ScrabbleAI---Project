#include "Player.h"
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

Player::Player (string const & name, size_t maxTiles, bool isAI) {

	p_name = name;
	hand_size = maxTiles;
	score = 0;
	AI = isAI;
}

Player::~Player () {}

set<Tile*> Player::getHandTiles() const {
	return hand;
}

bool Player::hasTiles(string const & move, bool resolveBlanks) const {

	set<Tile*> temp = hand;	//copy of hand
	set<Tile*>::iterator start;	//iterator
	char a;
	bool found;

	for (unsigned int i = 0; i < move.size(); ++i) { //iterate through string
		
		a = tolower(move[i]);
		found = false;	//flag if Tile found
		for (start = temp.begin(); start != temp.end(); ++start) {	//iterate through copy of hand

			if (a == tolower((*start)->getLetter())) {	//found the letter
				if ((*start)->isBlank() && resolveBlanks == true) {	//if PLACE is chosen and a blank is found
					++i;	// to avoid a true if statement every iteration after a '?' is found
				}
				temp.erase(start);
				found = true;
				break;
			}
		}

		if (found == false) {	//if a letter in move string isn't found
			return false;
		}
	}	

	return true;
}

vector<Tile*> Player::takeTiles (string const & move, bool resolveBlanks) {

	vector<Tile*> to_place;
	set<Tile*>::iterator start;
	char a;

	for (unsigned int i = 0; i < move.size(); ++i) {

		a = tolower(move[i]);
		for (start = hand.begin(); start != hand.end(); ++start) {
			if (a == ((*(start))->getLetter())) {

				if (resolveBlanks == true && (*(start))->isBlank()) {	//PLACE is chosen
					(*(start))->useAs(move[i+1]);	//set '?' use as next letter in string
					++i;	//to skip next iteration
					(to_place).push_back(*(start));	//add Tile* to vector
					hand.erase(start);				//erase letter from hand
					break;
				}

				else {
					(to_place).push_back(*(start));	//add Tile* to vector
					hand.erase(start);				//erase letter from hand
					break;
				}
			}
		}		
	}
	return to_place;
}

void Player::addTiles (vector<Tile*> const & tilesToAdd) {	//add tiles to player hand

	for (unsigned int i = 0; i < tilesToAdd.size(); i++) {
		hand.insert(tilesToAdd[i]);
	}
}

size_t Player::getMaxTiles() const {
	return hand_size;
}

void Player::addScore(int to_add) {	//add to player score

	score += to_add;
}

int Player::getScore() const{

	return score;
}

void Player::subtractScore(int to_subtract) {	//subtract from player score

	score -= to_subtract;
}

string Player::getName() const {
	return p_name;
}

bool Player::isAI() {
	return AI;
}
