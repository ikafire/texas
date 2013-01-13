#ifndef CARD_H
#define CARD_H

#include <string>

class Card {
public:
	enum Suit { EMPTY, Club, Diamond, Heart, Spade };
	Card();
	Card(const int value, const Suit suit);
	std::string toString() const;

	int getValue() const { return value; }
	Suit getSuit() const { return suit; }
private:
	int value;
	Suit suit;
};

# endif