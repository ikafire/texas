#ifndef HOLDEM_H
#define HOLDEM_H

#include <vector>
#include <deque>
#include <cassert>

#include "Deck.h"
#include "Card.h"
#include "Player.h"
#include "ComPlayer.h"
#include "HumanPlayer.h"
#include "Stage.h"

typedef unsigned money;
typedef unsigned player_num;

/*
The main part of Texas holdem game, controls game flow and lots of parameters.
To play the game, make an object of Holdem and call run().
*/
class Holdem {
public:
	const static player_num MIN_PLAYER = 2;
	const static player_num MAX_PLAYER = 9;

	Holdem();

	void run();
private:
	Deck deck;
	std::vector<Player*> playerList;	//List of all players
	std::vector<Player*> players;		//List of players still in game (not broke)
	std::vector<Player*> brokePlayers;	//List of broke players
	std::vector<Card> community;		//community cards
	money pot;				//sum of all side pots
	money sBlind;			//small blind amount
	money bBlind;			//big blind amount
	player_num numOfPlayers;//how many players in-game (broke players excluded)
	player_num dealer;		//dealer position
	player_num humanPos;	//human player position
	bool allowAllIn;		//if this game allow all-in or not
	unsigned roundNum;		//rount number of this game (start with 1)
	bool gotoShowDown;		//whether all but one players are all-in or not (skip betting if true)

	void constructPlayers(const money budget); //construct all players
	void startGame();
	std::vector<Player*> highestHands(std::vector<Player*> &competitors); //pick out player(s) with the highest hand
	void cleanUp(); //ready for next round of game

	//functions below are console-only

	bool setParams(money &budget);	//get and set game parameters
	bool preFlop();
	bool flop();
	bool turn();
	bool river();
	void showDown();
	bool betting(const Stage stage);
	void stageResult();				//output betting result
	void distributePot(std::deque< std::vector<Player*> > &ranks);
	void checkBroke();				//check if any player is broke
	bool checkEarlyEnd();			//check if all but one players are folded
	bool askContinue();				//ask if player want to player again
	void gameOver();
};

#endif
