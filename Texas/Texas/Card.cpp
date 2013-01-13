#include <string>
using std::string;

#include <sstream>
using std::stringstream;

#include <cassert>
#include "Card.h"

Card::Card() {
	value = 0;
	suit = EMPTY;
}

Card::Card(const int value,const Suit suit) {
	//check if card is invalid in debug mode
	assert(value > 0 && value <= 13);
	assert(suit == Card::Club || suit == Card::Diamond || suit == Card::Heart || suit == Card::Spade);

	this->value = value;
	this->suit = suit;
}

string Card::toString() const {
	stringstream sstr;

	switch (suit) {
	case Club:
		sstr << 'C';
		break;
	case Diamond:
		sstr << 'D';
		break;
	case Heart:
		sstr << 'H';
		break;
	case Spade:
		sstr << 'S';
		break;
	default:
		throw new std::exception("ERROR: toString() called on empty Card");
	}

	if (value >=2 && value <= 10) {
		sstr << value;
	} else {
		switch (value) {
		case 1:  sstr << 'A'; break;
		case 11: sstr << 'J'; break;
		case 12: sstr << 'Q'; break;
		case 13: sstr << 'K'; break;
		default:
			throw new std::exception("ERROR: Card value out of range");
		}
	}

	return sstr.str();
}