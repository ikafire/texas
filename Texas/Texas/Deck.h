#ifndef DECK_H
#define DECK_H

#include <string>
#include <vector>
#include "Card.h"

class Deck {
public:
	Deck();
	void reset();

	//Warning: if you call nextCard() more than 52 times without calling reset(), it will throw exception
	Card nextCard();
private:
	const static int NUM_OF_CARDS = 52;
	std::vector<Card> cardSet;
	std::vector<bool> isTaken;	//used to specify if certain card is taken
	int cardDealt;
};

#endif