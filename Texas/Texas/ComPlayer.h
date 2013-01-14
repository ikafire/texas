#ifndef COMPLAYER_H
#define COMPLAYER_H

#include "Player.h"

class ComPlayer : public Player {
public:
	enum Character {Radical, Normal, Calm};
	ComPlayer(const money budget, const bool allowAllIn) 
		: Player(budget, allowAllIn) {}
	virtual Action generateAction(const GameStatus status, money &raise, money &pay);
private:
	Character character;
	void setCharacter();
	Character getCharacter() const {return character;}

	int strength;
	void setStrength();

	void preflop(Action&, money &pay, money &raise, const GameStatus& );
	int estimationPre();
	void flop(Action&, money &pay, money &raise, const GameStatus& );
	int estimationFlop();
	void turn(Action&, money &pay, money &raise, const GameStatus& );
	int estimationTurn();
	void river(Action&, money &pay, money &raise, const GameStatus& );
	int estimationRiver();
};

#endif