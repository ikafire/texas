#ifndef HOLDEM_H
#define HOLDEM_H

#include <vector>
#include <cassert>

#include "Deck.h"
#include "Card.h"
#include "Player.h"

typedef unsigned money;
typedef unsigned player_num;

class Holdem {
public:
	void run();
	const static player_num MINPLAYER = 2;
	const static player_num MAXPLAYER = 9;
private:
	Deck deck;
	std::vector<Player> players;
	std::vector<Card> community;
	money pot;
	money sBlind;
	money bBlind;
	money currentBet;
	money minRaise;
	player_num numOfPlayers;
	player_num dealer;
	player_num lastRaise;
	bool allowAllIn;
	unsigned roundNum;

	void setParams(player_num &humanPos, money &budget); //console only
	void constructPlayers(const player_num humanPos, const money budget);
	void startGame();
};

#endif