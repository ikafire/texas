#ifndef DECK_H
#define DECK_H

#include <string>
#include <vector>
#include "Card.h"

class Deck {
public:
	Deck();
	void reset();
	//Warning: if you call nextCard() more than 52 times without calling reset(), it will loop forever (or throw error when in debug mode)
	Card nextCard();
private:
	std::vector<Card> cardSet;
	std::vector<bool> isTaken;	//used to specify if certain card is taken
	const static int numOfCards = 52;
	bool isEmpty();
};

#endif