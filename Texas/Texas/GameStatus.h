#ifndef GAMESTATUS_H
#define GAMESTATUS_H

#include <vector>

#include "Card.h"
#include "Stage.h"

typedef unsigned money;

/*
This class work as a relay of Holdem game status.
When class:Holdem try to call class:Player to react, it passes an object of this class as a set of game status parameters.
It's member functions are all inline functions, so no cpp files are needed.
*/
class GameStatus {
	friend class Holdem;
public:
	std::vector<Card> getCommunity() const { return community; }
	Stage getStage() const { return stage; }
	money getPot() const { return pot; }
	money getCurrentBet() const { return currentBet; }
	money getMinRaise() const { return minRaise; }
private:
	std::vector<Card> community; //0~5 community cards
	Stage stage; //which stage is the betting in (PreFlop, Flop, Turn, or River)
	money pot;
	money currentBet; //current bet on table
	money minRaise; //minimum raise limit
};

#endif
