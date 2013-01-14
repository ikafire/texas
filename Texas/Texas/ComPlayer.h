#ifndef COMPLAYER_H
#define COMPLAYER_H

#include "Player.h"
#include "Judge.h"
#include "GameStatus.h"

#include <string>

class ComPlayer : public Player {
public:
	enum Character {Radical, Normal, Coward};
	ComPlayer(const money budget, const bool allowAllIn, std::string name) 
		: Player(budget, allowAllIn, name) {}
	virtual Action generateAction(const GameStatus status, money &raise, money &pay);
private:
	Character character;
	void setCharacter();
	Character getCharacter() const {return character;}
	
	int confidence;
	void setConfidence();
	int getConfidence() const {return confidence;}

	money calculateRaise(const GameStatus&) const;
	void helpAccount(const money &pay);

	int estimationPre() const;
	int estimationFlopAndTurn(const GameStatus&) const;
	Action preflop(money &pay, money &raise, const GameStatus& );
	Action flopAndTurn(money &pay, money &raise, const GameStatus& );
	Action river(money &pay, money &raise, const GameStatus& );
};

#endif