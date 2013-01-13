#include <string>
using std::string;

#include <vector>
using std::vector;

#include <random>
using std::srand;
using std::rand;

#include <ctime>
using std::time;

#include<cassert>
#include "Deck.h"
#include "Card.h"

Deck::Deck() : cardSet(NUM_OF_CARDS) {
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

//Warning: if you call nextCard() more than 52 times without calling reset(), it will throw exception
Card Deck::nextCard() {
	if (cardDealt >= NUM_OF_CARDS) {
		throw new std::exception("ERROR: more than 52 cards are dealt.");
	}

	int nextPos;
	do {
		nextPos = rand() % NUM_OF_CARDS;
	} while (isTaken[nextPos]);	//if the card in generated position is taken, generate another one

	isTaken[nextPos] = true;
	++cardDealt;
	return cardSet[nextPos];
}

//#include <iostream>
//using std::cout;
//using std::endl;
//
//int main() {
//	Deck deck;
//	for (int i=0; i<52; ++i) {
//		cout << deck.nextCard().toString() << endl;
//	}
//	deck.reset();
//	cout << endl;
//	for (int i=0; i<52; ++i) {
//		cout << deck.nextCard().toString() << endl;
//	}
//	system("pause");
//}