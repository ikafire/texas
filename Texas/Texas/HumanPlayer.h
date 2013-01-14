#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"

class HumanPlayer : public Player {
public:
	HumanPlayer(const money budget, const bool allowAllIn, std::string name) 
		: Player(budget, allowAllIn, name) {}
	Action generateAction(const GameStatus status, money &raise, money &pay);
private:
	Action getInput();
	bool actionValid(Action act, const GameStatus &status, money &raise);
};

#endif