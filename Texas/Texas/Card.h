#ifndef CARD_H
#define CARD_H

#include <string>

/* 
This class work as, well, card.
Contains value, suit, and a function to convert into string
*/
class Card {
public:
	enum Suit { EMPTY, Club, Diamond, Heart, Spade };

	Card();
	Card(const int value, const Suit suit);

	//convert card data into string for output
	std::string toString() const;

	int getValue() const { return value; }
	Suit getSuit() const { return suit; }
private:
	int value;
	Suit suit;
};

# endif
