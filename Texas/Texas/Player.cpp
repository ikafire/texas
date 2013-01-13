#include <vector>
using std::vector;

#include <cassert>

#include "Card.h"
#include "Player.h"

Player::Player(const money budget, const bool allowAllIn) 
	: ALLOW_ALL_IN(allowAllIn)
{
	wallet = budget;
}

void Player::receiveCards(Card c1, Card c2) {
	assert(pocket.empty());

	pocket.push_back(c1);
	pocket.push_back(c2);
}

void Player::blind(const money amount) {
	assert(!allIn && !folded);
	assert(stageBet == 0 && totalBet == 0);
	if (wallet < amount) {
		throw new std::exception("ERROR: not enough money to blind bet");
	}

	wallet -= amount;
	stageBet = amount;
	totalBet = amount;
}

void Player::win(const money amount) {
	assert(!folded);
	
	wallet += amount;
}

void Player::nextStage() {
	stageBet = 0;
}

void Player::nextRound() {
	pocket.clear();
	stageBet = 0;
	totalBet = 0;
	allIn = false;
	folded = false;
}