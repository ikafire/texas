#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"
#include "GameStatus.h"

typedef unsigned money;

class Player {
public:
	enum Action { Check, Call, Raise, AllIn, Fold };
	Player(const money budget, const bool allowAllIn);
	void receiveCards(Card c1, Card c2);
	void blind(const money amount);
	void win(const money amount);
	void nextStage();
	void nextRound();

	money getWallet() const { return wallet; }
	money getStageBet() const { return stageBet; }
	money getTotalBet() const { return totalBet; }
	bool isAllIn() const { return allIn; }
	bool isFolded() const { return folded; }

	virtual Action generateAction(const GameStatus status, money &raise, money &pay) {
		throw new std::exception("ERROR: generateAction() not implemented");
	}
protected:
	const bool ALLOW_ALL_IN;
	std::vector<Card> pocket;
	money wallet;
	money stageBet;
	money totalBet;
	bool allIn;
	bool folded;
};

#endif