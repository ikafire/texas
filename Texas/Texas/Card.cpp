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

	assert(value > 0 && value <= 13);
	assert(suit == Card::club || suit == Card::diamond || suit == Card::heart || suit == Card::spade);
	//check if card is invalid in debug mode

	this->value = value;
	this->suit = suit;
}

string Card::toString() {
	stringstream sstr;
	
	switch (suit) {
	case club:
		sstr << 'C';
		break;
	case diamond:
		sstr << 'D';
		break;
	case heart:
		sstr << 'H';
		break;
	case spade:
		sstr << 'S';
		break;
	default:
		sstr << "ERROR";
		break;
	}
	sstr << value;

	return sstr.str();
}

//#include <iostream>
//
//int main() {
//	Card c1(1,Card::diamond), c2;
//	std::cout << c1.toString() << std::endl << c2.toString() << std::endl;
//	std::cout << c1.getSuit();
//	system("pause");
//}