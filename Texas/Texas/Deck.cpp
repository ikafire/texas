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

Deck::Deck() {
	for (int s=1; s<=4; ++s) {
		for (int v=1; v<=13; ++v) {
			Card::Suit suit;
			switch (s) {
			case 1: suit = Card::club; break;
			case 2: suit = Card::diamond; break;
			case 3: suit = Card::heart; break;
			case 4: suit = Card::spade; break;
			}
			Card c(v, suit);
			cardSet.push_back(c);
			isTaken.push_back(false);
		}
	}
	srand(time(0));

	assert(cardSet.size()==numOfCards && isTaken.size()==numOfCards);
}

void Deck::reset() {
	for (vector<bool>::iterator iter = isTaken.begin(); iter != isTaken.end(); ++iter) {
		*iter = false;
	}
}

//Warning: if you call nextCard() more than 52 times without calling reset(), it will loop forever (or throw exception when in debug mode)
Card Deck::nextCard() {

	assert(!isEmpty());
	//when flag NDEBUG is not defined, it will check if deck is empty everytime when nextCard() is called
	//if deck is empty while nextCard is called, it will throw an exception

	int nextPos;
	do {
		nextPos = rand()%numOfCards;
	} while (isTaken[nextPos]);	//if the card in generated position is taken, generate another one

	isTaken[nextPos] = true;
	return cardSet[nextPos];
}

bool Deck::isEmpty() {
	for (vector<bool>::iterator iter = isTaken.begin(); iter != isTaken.end(); ++iter) {
		if (*iter==false) {
			return false;
		}
	}
	return true;
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