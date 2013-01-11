#ifndef CARD_H
#define CARD_H

#include <string>

class Card {
public:
	enum Suit { EMPTY, club, diamond, heart, spade };
	Card();
	Card(int value, Suit suit);
	inline int getValue() { return value; }
	inline Suit getSuit() { return suit; }
	std::string toString();
private:
	int value;
	Suit suit;
};

# endif