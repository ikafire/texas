#include <string>
using std::string;

#include <vector>
using std::vector;

#include <cstdlib>
using std::srand;
using std::rand;

#include <ctime>
using std::time;

#include<cassert>
#include "Deck.h"
#include "Card.h"

Deck::Deck() {
	//construct 52 cards
	for (int s=1; s<=4; ++s) {
		for (int v=1; v<=13; ++v) {
			Card::Suit suit;
			switch (s) {
			case 1: suit = Card::Club; break;
			case 2: suit = Card::Diamond; break;
			case 3: suit = Card::Heart; break;
			case 4: suit = Card::Spade; break;
			}
			Card c(v, suit);
			cardSet.push_back(c);
			isTaken.push_back(false);
		}
	}

	srand(time(0));
	cardDealt = 0;

	assert(cardSet.size()==NUM_OF_CARDS && isTaken.size()==NUM_OF_CARDS);
}

void Deck::reset() {
	for (vector<bool>::iterator iter = isTaken.begin(); iter != isTaken.end(); ++iter) {
		*iter = false;
	}
	cardDealt = 0;
}

//Warning: if call nextCard() more than 52 times without calling reset(), it will throw exception
Card Deck::nextCard() {
	if (cardDealt >= NUM_OF_CARDS) {
		throw new std::exception();
	}

	int nextPos;
	do {
		nextPos = rand() % NUM_OF_CARDS;
	} while (isTaken.at(nextPos));	//if the card in generated position is taken, generate another one

	isTaken.at(nextPos) = true;
	++cardDealt;
	return cardSet.at(nextPos);
}
