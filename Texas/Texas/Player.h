#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "Card.h"
#include "GameStatus.h"
#include "PokerHand.h"

typedef unsigned money;

/*
This is the player class, which is a abstract class and base class for HumanPlayer and ComPlayer.
It contains almost every fonction and parameter a player need to play Texas holdem, except for generateAction(), which is a pure virtual function.
HumanPlayer and ComPlayer overrides generateAction() to act differently when betting, one acts according to user input and the other relying on AI.
*/
class Player {
public:
	enum Action { Check, Call, Raise, AllIn, Fold };

	Player(const money budget, const bool allowAllIn, std::string name);

	void receiveCards(Card c1, Card c2);	//receive pocket cards
	void blind(const money amount);			//make blind bet
	void win(const money amount);			//win money
	void nextStage();						//prepare for next stage
	void nextRound();						//prepare for next round of game
	void calcHand(const std::vector<Card> &community);	//calculate how high is its hand (pocket + community)
	bool isBroke(money bigBlind);			//check if player is broke

	money getWallet() const { return wallet; }
	money getStageBet() const { return stageBet; }
	money getTotalBet() const { return totalBet; }
	bool isAllIn() const { return allIn; }
	bool isFolded() const { return folded; }
	std::string getName() const { return name; }
	std::vector<Card> getPocket() const { return pocket; }
	PokerHand getHand() const { return hand; }

	virtual Action generateAction(const GameStatus status, money &raise, money &pay) = 0;
protected:

	const bool ALLOW_ALL_IN;	//if the game allows all-in
	std::vector<Card> pocket;	//his pocket cards
	money wallet;				//his wallet
	money stageBet;				//his bet in current stage
	money totalBet;				//his total bet in this round of game
	bool allIn;					//if he's all-in
	bool folded;				//if he folded
	bool broke;					//if he's broke
	std::string name;			//his name
	PokerHand hand;				//his hand height
};

#endif
