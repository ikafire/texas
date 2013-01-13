#ifndef HOLDEM_H
#define HOLDEM_H

#include <vector>
#include <cassert>

#include "Deck.h"
#include "Card.h"
#include "Player.h"
#include "Stage.h"

typedef unsigned money;
typedef unsigned player_num;

class Holdem {
public:
	const static player_num MIN_PLAYER = 2;
	const static player_num MAX_PLAYER = 9;
	void run();
private:
	Deck deck;
	std::vector<Player> players;
	std::vector<Card> community;
	money pot;
	money sBlind;
	money bBlind;
	player_num numOfPlayers;
	player_num dealer;
	player_num humanPos;
	bool allowAllIn;
	unsigned roundNum;

	void constructPlayers(const money budget);
	void startGame();
	//functions below are console-only
	bool setParams(money &budget);
	void preFlop();
	void flop();
	void turn();
	void river();
	bool showDown();
	void betting(const player_num startPos, Stage stage, money minRaise);
};

#endif