#ifndef COMPLAYER_H
#define COMPLAYER_H

#include "Player.h"

class ComPlayer : public Player {
public:
	ComPlayer(const money budget, const bool allowAllIn) 
		: Player(budget, allowAllIn) {}
	Action generateAction(const GameStatus status, money &raise, money &pay);
private:

};

#endif