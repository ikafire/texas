#ifndef DECK_H
#define DECK_H

#include <string>
#include <vector>
#include "Card.h"

/*
This class work as a shuffled 52-card deck.
By calling nextCard(), it can return 52 random cards without repeating.
When nextCard() called more than 52 times without resetting, an exception will be thrown.
To reset the deck, call reset().
*/
class Deck {
public:
	Deck();

	void reset();
	//Warning: if you call nextCard() more than 52 times without calling reset(), it will throw exception
	Card nextCard();
private:
	const static int NUM_OF_CARDS = 52;
	std::vector<Card> cardSet; //52 cards
	std::vector<bool> isTaken; //used to specify if certain card is taken
	int cardDealt;
};

#endif
