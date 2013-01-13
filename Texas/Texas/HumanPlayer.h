#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"

class HumanPlayer : public Player {
public:
	HumanPlayer(const money budget, const bool allowAllIn) 
		: Player(budget, allowAllIn) {}
	Action generateAction(const GameStatus status, money &raise, money &pay);
private:

};

#endif