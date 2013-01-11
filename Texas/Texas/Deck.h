#ifndef DECK_H
#define DECK_H

#include <string>
#include <vector>
#include "Card.h"

class Deck {
public:
	Deck();
	void shuffle();
	Card nextCard();
private:
	std::vector<Card> cardSet;
	std::vector<int> getOrder;
};

#endif