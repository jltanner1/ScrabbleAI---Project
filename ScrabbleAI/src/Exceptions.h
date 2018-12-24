/*
 * Exceptions.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* This file contains custom-defined exceptions thrown to signal errors
   or problems in various parts of the code.
   All of them have a constructor with a string, and the string can
   be retrieved using the getMessage() function (not what()).
   The specific strings that are used to signal particular errors are
   discussed where they are thrown.
*/

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <stdexcept>
#include <string>

/* This exception is thrown when there are problems opening data files,
   or when data files have format errors. */

class FileException : public std::exception {
public:
	FileException (std::string message) { _message = message; }
	~FileException () throw () {}
	std::string getMessage () const { return _message; }
private:
	std::string _message;
};

//Exception thrown when not bag does not have enough tiles left to exchange
class NotEnoughTilesInBag : public std::exception {
public:
   NotEnoughTilesInBag (std::string message) { _message = message; }
   ~NotEnoughTilesInBag () throw () {}
   std::string getMessage () const { return _message; }
private:
   std::string _message;
};
//Exception thrown when necessary tiles to make move are not in hand
class dontHaveTiles : public std::exception {
public:
   dontHaveTiles (std::string message) { _message = message; }
   ~dontHaveTiles () throw () {}
   std::string getMessage () const { return _message; }
private:
   std::string _message;
};

//Excpetion thrown when used is not starting on the starting square
class notTouchingStartingSquareException : public std::exception {
public:
   notTouchingStartingSquareException (std::string message) { _message = message; }
   ~notTouchingStartingSquareException () throw () {}
   std::string getMessage () const { return _message; }
private:
   std::string _message;
};

//Exception thrown when starting position of PlaceMove will make word (to be placed) go out of bounds
class positionOutOfBounds : public std::exception {
public:
   positionOutOfBounds (std::string message) { _message = message; }
   ~positionOutOfBounds () throw () {}
   std::string getMessage () const { return _message; }
private:
   std::string _message;
};

//Exception thrown when starting position of PlaceMove is already occupied
class firstSpaceAlreadyOccupied : public std::exception {
public:
   firstSpaceAlreadyOccupied (std::string message) { _message = message; }
   ~firstSpaceAlreadyOccupied () throw () {}
   std::string getMessage () const { return _message; }
private:
   std::string _message;
};

//Exception thrown when there are no valid word possiblities with given PlaceMove
class noValidWordPossibilites : public std::exception {
public:
   noValidWordPossibilites (std::string message) { _message = message; }
   ~noValidWordPossibilites () throw () {}
   std::string getMessage () const { return _message; }
private:
   std::string _message;
};

//Exception thrown when PlaceMove does not touch any other letters on the board
class noAdjacentLettersDetected : public std::exception {
public:
   noAdjacentLettersDetected (std::string message) { _message = message; }
   ~noAdjacentLettersDetected () throw () {}
   std::string getMessage () const { return _message; }
private:
   std::string _message;
};

//Exception thrown when an invalid word is detected in vector of pairs
class invalidWordException : public std::exception {
public:
   invalidWordException (std::string message) { _message = message; }
   ~invalidWordException () throw () {}
   std::string getMessage () const { return _message; }
private:
   std::string _message;
};

//Exception thrown when there are no tiles left in the bag
class bagEmpty : public std::exception {
public:
   bagEmpty (std::string message) { _message = message; }
   ~bagEmpty () throw () {}
   std::string getMessage () const { return _message; }
private:
   std::string _message;
};

#endif /* EXCEPTIONS_H_ */