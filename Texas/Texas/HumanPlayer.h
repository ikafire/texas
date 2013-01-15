#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"

/*
Inherited from class:Player, this class receives user input when betting, and check its validity.
*/
class HumanPlayer : public Player {
public:
	HumanPlayer(const money budget, const bool allowAllIn, std::string name) 
		: Player(budget, allowAllIn, name) {}

	virtual Action generateAction(const GameStatus status, money &raise, money &pay);
private:
	Action getInput(); //get input from player
	bool actionValid(Action act, const GameStatus &status, money &raise); //check the action's validity
};

#endif
