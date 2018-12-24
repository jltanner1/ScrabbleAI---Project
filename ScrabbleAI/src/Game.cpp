#include "Game.h"
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

Game::Game(size_t h_size, string t_file, string d_file, string b_file, uint32_t _s, string i_file) {
    
	_handsize = h_size;

	_seed = _s;

	_bag = new Bag(t_file, _seed);	//create a bag of tiles

	_board = new Board(b_file);	//create a board

	_dict = new Dictionary(d_file);	//create a dictionary

	_trie = new TrieSet();	//create a trie
	
	_trie->fill(_dict);		//construct trie with dictionary words

	_player_vec = makePlayers();	//create a vector of players pointers

	std::vector<Tile*> temp_drawn;	//vector to hold drawn tiles for each player

	for (unsigned int i = 0; i < _player_vec.size(); ++i) {	//fill each player hand with randome tiles
		temp_drawn = _bag->drawTiles(_handsize);
		_player_vec[i]->addTiles(temp_drawn);
	}

	if (i_file != "") _board->init_board(i_file);
}

Game::~Game () {
	for (unsigned int i = 0; i < _player_vec.size(); ++i) {

		set<Tile*> temp = _player_vec[i]->getHandTiles();

		for (set<Tile*>::iterator delete_it = temp.begin(); delete_it != temp.end(); ++delete_it) {
			delete (*delete_it);	//delete tiles left in player hand
		}

		delete _player_vec[i];	//delete player pointer
	}

	delete _bag;	//delete bag
	delete _board;	//delete board
	delete _dict;	//delete dictionary
	delete _trie;	//delete trie
}

vector<Player*> Game::makePlayers() {	//to create players for the game
	
	vector<Player*> temp;
	int num_players;
	cout << "Please enter number of players (1-8): ";
	cin >> num_players;

	while (num_players > 8 || num_players < 1) {

		cout << "Enter a number of between 1 and 8: ";
		cin >> num_players;
	}

	string name;
	getline(cin, name);

	for (int i = 0; i < num_players; i++) {

		Player* current;

		cout << "Enter a name for Player " << i+1 << ": ";
		getline(cin, name);

		if (name.size() >= 4 && std::tolower(name[0]) == 'c' && std::tolower(name[1]) == 'p' && std::tolower(name[2]) == 'u') {

			if (std::tolower(name[3]) == 's') current = new AI(name, _handsize, true, false);

			else if (std::tolower(name[3]) == 'l') current = new AI(name, _handsize, true, true);
		}
		else current = new Player(name, _handsize, false);

		temp.push_back(current);
	}
	cout << "Press enter to continue" << endl;
	getline(cin,name);
	return temp;
}	

void Game::printScores() {	//print scores throughout the game

	cout << "Scores: " << endl;
	cout << endl;
	for (unsigned int i = 0; i < _player_vec.size(); ++i) {

		cout << _player_vec[i]->getName() << ": " << _player_vec[i]->getScore() << endl;
	}
}

void Game::runGame() {	//the continuous loop of the game
	
	bool game_running = true;
	unsigned int current_player = 0;
	unsigned int total_players = _player_vec.size();	
	string input;

	bool bagEmptybool = false;			//to keep track if bag is empty
	int player_out_of_tiles = -1;		//is set to the player who runs out of tiles first
	unsigned int pass_count = 0;		//counts number of passes in a row

	cout << endl;	//to account for whitespace
	Move* m = NULL;

	while (game_running) {

		p.printBoard(*(_board));
		p.printHand(*(_player_vec)[current_player]);
		printScores();
		cout << endl;

		if (_player_vec[current_player]->isAI() == false) { //human player chooses what to do
			cout << _player_vec[current_player]->getName() << ": Pass, Exchange, or Place?  ";
			getline(cin, input);
		}

		try {

			if(_player_vec[current_player]->isAI()) {	//AI turn
                
				pair<Move*,int> resultAI = Move::parseAIMoves(_board, _trie, _player_vec[current_player], _bag, _dict);
				m = resultAI.first;

				if (m->isPass()) {
					++pass_count;
				}
				else {
					pass_count = 0;
				}

				PlaceMove* current = dynamic_cast<PlaceMove*>(m);

				if (m->isPass() == false) {
                    
					current->fillVec(current->getString());
                    
					_board->executePlaceMove(*current);	//place tiles for move
                    
				 	current->drawAndScore(resultAI.second, (*current), (*_bag));	//update AI score and hand
				}
				else m->execute((*_board), (*_bag), (*_dict)); //AI executes Pass
			}

			else {	//Human turn
				m = Move::parseMove(input, *(_player_vec[current_player]));

				if (m->isPass()) {
					++pass_count;
				}
				else {
					pass_count = 0;
				}

				m->execute((*_board), (*_bag), (*_dict));
			}
		}
		catch (NotEnoughTilesInBag& e) {
			cout << e.getMessage() << endl;
			cout << "Player " << current_player+1 << ": please enter a valid move" << endl;
			continue;
		}
		catch (dontHaveTiles& e) {	
			cout << e.getMessage() << endl;
			cout << "Player " << current_player+1 << ": please enter a valid move" << endl;
			continue;
		}
		catch (notTouchingStartingSquareException& e){
			cout << e.getMessage() << endl;
			cout << "Player " << current_player+1 << ": please enter a valid move" << endl;
		 	continue;
		}
		catch (positionOutOfBounds& e) {
			cout << e.getMessage() << endl;
			cout << "Player " << current_player+1 << ": please enter a valid move" << endl;
			continue;
		}
		catch (firstSpaceAlreadyOccupied& e) {
			cout << e.getMessage() << endl;
			cout << "Player " << current_player+1 << ": please enter a valid move" << endl;
			continue;
		}
		catch(noValidWordPossibilites& e) {
			cout << e.getMessage() << endl;
			cout << "Player " << current_player+1 << ": please enter a valid move" << endl;
			continue;
		}
		catch(noAdjacentLettersDetected& e) {
			cout << e.getMessage() << endl;
			cout << "Player " << current_player+1 << ": please enter a valid move" << endl;
			continue;
		}
		catch(invalidWordException& e) {
			cout << e.getMessage() << endl;
			cout << "Player " << current_player+1 << ": please enter a valid move" << endl;
			continue;
		}
		catch(bagEmpty& e) {
			cout << e.getMessage() << endl;
			bagEmptybool = true;
		}

		delete m;
        
		if (bagEmptybool == true && _player_vec[current_player]->getHandTiles().size() == 0) {
			player_out_of_tiles = current_player;
			game_running = false;
		}

		if (pass_count == total_players) {	//if every player passes in succession
			game_running = false;
		}

		current_player = (current_player+1)%total_players;
		
	}	
	
	//end game conditions have been met
        
	set<Tile*>::iterator it;
	set<Tile*> temp;

	int totalSubtracted = 0;
	for (unsigned int i = 0; i < _player_vec.size(); ++i) {

		if ((int)i != player_out_of_tiles) {	//to skip the player who has no tiles remaining if this case is met
			
			temp = _player_vec[i]->getHandTiles();
			int subtract;

			for (it = temp.begin(); it != temp.end(); ++it) {

				subtract = ((*it)->getPoints());
				totalSubtracted += subtract;
				_player_vec[i]->subtractScore(subtract);
			}
		}
	}

	if (player_out_of_tiles != -1) _player_vec[player_out_of_tiles]->addScore(totalSubtracted);
}

void Game::printFinalScores() {	//final outcome of the game

	int max_score = -1000;

	cout << "Final Scores: " << endl;
	cout << endl;

	for (unsigned int i = 0; i < _player_vec.size(); ++i) {	//find the max score 

		cout << _player_vec[i]->getName() << ": " << _player_vec[i]->getScore() << endl;

		if (_player_vec[i]->getScore() >= max_score) {
			max_score = _player_vec[i]->getScore();
		}
	}

	cout << endl;
	cout << "Winner(s):" << endl;	

	for (unsigned int j = 0; j < _player_vec.size(); ++j) {		//print out winners

		if (_player_vec[j]->getScore() == max_score) {

			cout << '\t' <<_player_vec[j]->getName() << endl;
		}
	}
	cout << endl;
}
