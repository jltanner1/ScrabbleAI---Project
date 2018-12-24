/*
 * Move.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* The Move class family encodes a move made by one player, including its type
   (PASS, EXCHANGE, PLACE), the tiles used (and use for blanks),
   start square, and direction (if needed).
   Call Move::parseMove() to turn an entered string into its subclass of
   Move, then call apply() on that returned move to have it execute.*/

#ifndef MOVE_H_
#define MOVE_H_

#include <vector>
#include <string>
#include "Tile.h"
#include "Player.h"
#include "Bag.h"
#include "Dictionary.h"
#include "Exceptions.h"
#include "Trie.h"
#include "AI.h"

// forward declaration to prevent circular includes
class Board;

class PlaceMove;

class AI;

class Trie;

class Move
{

public:

	//generates a move for an AI player based on their type, passes if no valid moves can be generated
	static std::pair<Move*,int> parseAIMoves(Board* _board, TrieSet* _trie, Player* p, Bag* _bag, Dictionary* _dict);

	/* Parses the given move m, and constructs the corresponding move subclass.
	   m is assumed to be in the format described on the assignment sheet for a move command.
	   The player passed in is used to verify that the proposed tiles are
	   in fact in the player's hand.
	   It can handle all three types of move (PASS, EXCHANGE, PLACE).
	   Coordinates start with 1.
	*/

	static Move* parseMove(std::string moveString, Player &p);


	/* Returns true iff the move is a PASS move */
	virtual bool isPass () const
	{
		return false;
	}

	/* Returns true iff the move is an EXCHANGE move */
	virtual bool isExchange () const
	{
		return false;
	}

	/* Returns true iff the move is a PLACE move */
	virtual bool isWord () const
	{
		return false;
	}

	/* Executes this move, whichever type it is.
	   This may throw exceptions; students: it's up to you to
	   decide (and document) what exactly it throws*/
	virtual std::pair<int,int> execute(Board & board, Bag & bag, Dictionary & dictionary) = 0;

	//virtual std::pair<Move*,int> execute

	virtual ~Move();

	//Add more public/protected/private functions/variables here.

protected:

	Player * _player;

	// constructor -- only accessible to subclasses
	Move(Player * player);

};

// Represents a pass move, where the player takes no action
class PassMove : public Move
{

public:

	/* Constructs a pass move. */
	PassMove(Player * player);

	/* Returns true iff the move is a PASS move */
	bool isPass () const
	{
		return true;
	}

	/* Executes this move, whichever type it is.
	   This may throw exceptions; students: it's up to you to
	   decide (and document) what exactly it throws*/
	std::pair<int,int> execute(Board & board, Bag & bag, Dictionary & dictionary);

	//Add more public/protected/private functions/variables here.

};

// represents an exchange move, were a player replaces certain tiles
class ExchangeMove : public Move
{

public:
	/* Creates an EXCHANGE move, exchanging the tiles listed in the
	   string (formatted according to the EXCHANGE command description)
	   with new tiles from the bag.
	   */
	ExchangeMove(std::string tileString, Player * p);

	/* Returns true iff the move is an EXCHANGE move */
	bool isExchange () const
	{
		return true;
	}

	/* Executes this move, whichever type it is.
	   This may throw exceptions; students: it's up to you to
	   decide (and document) what exactly it throws*/
	std::pair<int,int> execute(Board & board, Bag & bag, Dictionary & dictionary);

	//Add more public/protected/private functions/variables here.

private:
	std::string input;

};

// represents a place move, where a player places one or more tiles onto the board.
class PlaceMove : public Move
{

public:
	/* Creates a PLACE move, starting at row y, column x, placing the tiles
	   described by the string tileString. If "horizontal" is true, then the tiles
	   are placed horizontally, otherwise vertically.
	   Coordinates start with 1.
	   The string m is in the format described in HW4; in particular, a '?'
	   must be followed by the letter it is to be used as.
	*/
	PlaceMove (size_t x, size_t y, bool horizontal, std::string tileString, Player * p);

	/* Returns true iff the move is a PLACE move */
	bool isWord () const
	{
		return true;
	}

	/* Returns the vector of tiles associated with a PLACE/EXCHANGE move.
	   Return value could be arbitrary for PASS moves. */
	std::vector<Tile*> const & tileVector () const;

	/* Executes this move, whichever type it is.
	   This may throw exceptions; students: it's up to you to
	   decide (and document) what exactly it throws*/
	std::pair<int,int> execute(Board & board, Bag & bag, Dictionary & dictionary);

	void drawAndScore(int _sum, PlaceMove &m, Bag & bag);

	//return the string of a PlaceMove
	std::string getString() const;

	// returns the starting row of a PlaceMove
	size_t getY() const;

	// return the starting column of a PlaceMove
	size_t getX() const;

	//returns true if PlaceMove is moving horizontal; false otherwise
	bool getDir() const;

	//return the size of tileString
	size_t getSize() const;

	//return player pointer
	Player* getP() const;

	//to delete all contents of place_vec
	void clearVec();

	//if move is invalid, puts contents of place_vec back into player hand
	void addBack();

	//used for AI's to fill the vector of tiles to place
	void fillVec(std::string input);

	//Add more public/protected/private functions/variables here.
private:
	std::string input;
	size_t x_start, y_start;
	bool dir;
	std::vector<Tile*> place_vec;
};



#endif /* MOVE_H_ */
