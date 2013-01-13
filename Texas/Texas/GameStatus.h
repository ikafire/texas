#ifndef GAMESTATUS_H
#define GAMESTATUS_H

#include <vector>

#include "Card.h"
#include "Stage.h"

typedef unsigned money;

class GameStatus {
	friend class Holdem;
public:
	std::vector<Card> getCommunity() const { return community; }
	Stage getStage() const { return stage; }
	money getPot() const { return pot; }
	money getCurrentBet() const { return currentBet; }
	money getMinRaise() const { return minRaise; }
private:
	std::vector<Card> community;
	Stage stage;
	money pot;
	money currentBet;
	money minRaise;
};

#endif