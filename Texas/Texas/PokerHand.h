#ifndef POKERHAND_H
#define POKERHAND_H

#include <string>
#include "Card.h"

class PokerHand
{
public:
	//各個牌型的數值,數值越大表示牌型越大
	static const int royalStraightFlush = 10;
	static const int straightFlush = 9;
	static const int fourOfAKind = 8;
	static const int fullHouse = 7;
	static const int flush = 6;
	static const int straight = 5;
	static const int threeOfAKind = 4;
	static const int twoPair = 3;
	static const int onePair = 2;
	static const int highCard = 1;

	PokerHand(const Card (&pickList)[5],const int);
	int getOrder() const;

	bool operator== (const PokerHand& h) const;
	bool operator!= (const PokerHand& h) const {return !(h == (*this));}
	bool operator> (const PokerHand& h) const;
	bool operator>= (const PokerHand& h) const {return (h == (*this)) || (*this) > h; }
	bool operator< (const PokerHand& h) const {return !((*this) >= h ); }
	bool operator<=(const PokerHand& h) const {return !((*this) > h);}

	std::string toString();


private:
	void sortSuit();
	int order;
	Card hand[5]; 
};

#endif