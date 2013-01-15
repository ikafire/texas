#include <vector>
using std::vector;

#include <string>
using std::string;

#include <cassert>

#include "Card.h"
#include "Player.h"
#include "Judge.h"

Player::Player(const money budget, const bool allowAllIn, string name) 
	: ALLOW_ALL_IN(allowAllIn)
{
	wallet = budget;
	this->name = name;
	stageBet = 0;
	totalBet = 0;
	allIn = false;
	folded = false;
	broke = false;
	hand = PokerHand();
}

void Player::receiveCards(Card c1, Card c2) {
	assert(pocket.empty());

	pocket.push_back(c1);
	pocket.push_back(c2);
}

void Player::blind(const money amount) {
	assert(!allIn && !folded);
	assert(stageBet == 0 && totalBet == 0);
	//the player should have enough money to make blind bet
	if (wallet < amount) {
		throw new std::exception();
	}

	wallet -= amount;
	stageBet = amount;
	totalBet = amount;

	if (wallet == 0) {
		allIn = true;
	}
}

void Player::win(const money amount) {
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
	hand = PokerHand();
}

void Player::calcHand(const vector<Card> &community) {
	//combine community cards with pocket cards
	vector<Card> cards(community);
	cards.insert(cards.end(), pocket.begin(), pocket.end());

	assert(cards.size() == 7);

	//determine how high is his hand
	hand = Judge::determineHand(cards);
}

bool Player::isBroke(money bigBlind) {
	if (broke) return true;

	if (wallet < bigBlind && !allIn) {
		broke = true;
	}

	return broke;
}
