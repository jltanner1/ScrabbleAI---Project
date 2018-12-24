/*
 * Player.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* Encodes the state of a player, mostly the name, tiles, points. 
   The main non-trivial ability of this class is to produce a vector
   of Tile* from a string describing tiles to place/exchange. */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <set>
#include <vector>
#include "Exceptions.h"
#include "Tile.h"


class Player
{

public:
	/* Constructor giving the player the given name, and setting their points to 0.
	   Does not give the player any tiles.
	*/
	Player (std::string const & name, size_t maxTiles, bool isAI);

	/* Destructor for a player. Deletes all the tiles the player still has. */
	virtual ~Player ();

	/* Returns the set of tiles the player currently holds. */
	std::set<Tile*> getHandTiles() const;

	/* Reads a move string and confirms that the player has the tiles necessary to
	   execute this move.

	   The move string is case-insensitive.

	   The boolean resolveBlanks states whether this is a PLACE or EXCHANGE move.
	   If resolveBlanks is true, then when the string contains a '?',
	   the next letter is interpreted as the letter to use the blank as.

	   By definition, if this function returns true, then takeTiles() would
	   succeed.
	 */

	bool hasTiles(std::string const & move, bool resolveBlanks) const;


	/* Reads a move string, finds the corresponding tiles in the player's hand, and
	   removes and returns those tiles in the order they appear in the move string.

	   The move string is case-insensitive.

	   The boolean resolveBlanks states whether this is a PLACE or EXCHANGE move.
	   If resolveBlanks is true, then when the string contains a '?',
	   the next letter is interpreted as the letter to use the blank as,
	   and the "use" field of the tile is set accordingly.

	   The move string is assumed to have correct syntax.
	*/
	std::vector<Tile*> takeTiles (std::string const & move, bool resolveBlanks);

	// Adds all the tiles in the vector to the player's hand.
	void addTiles (std::vector<Tile*> const & tilesToAdd);

	// returns hand_size of player
	size_t getMaxTiles() const;

	//adds value to player score
	void addScore(int to_add);

	//return player score
	int getScore() const;

	//subtracts value from player score
	void subtractScore(int to_subtract);

	//returns player name
	std::string getName() const;

	//return true if player is an AI
	bool isAI();

	//Add more public/protected/private functions/variables here.
private:

	std::string p_name;		//player name
	std::set<Tile*> hand;	//player hand
	size_t hand_size;		//size of hand
	int score;				//score of player
	bool AI;				//true if player is an AI
};


#endif /* PLAYER_H_ */
