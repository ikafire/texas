#ifndef COMPLAYER_H
#define COMPLAYER_H

#include "Player.h"

class ComPlayer : public Player {
public:
	ComPlayer(const money budget, const bool allowAllIn, std::string name) 
		: Player(budget, allowAllIn, name) {}
	Action generateAction(const GameStatus status, money &raise, money &pay);
private:

};

#endif