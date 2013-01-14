#ifndef POKERHAND_H
#define POKERHAND_H

#include <vector>
using std::vector;
#include <string>
using std::string;
#include "Card.h"


class PokerHand
{
public:
	enum Order {EMPTY, HighCard, OnePair, TwoPairs, ThreeOfAKind, Straight, Flush, FullHouse, FourOfAKind, StraightFlush, RoyalStraightFlush};
	
	PokerHand(){};
	PokerHand(const Order order, const vector<Card>& pickList);
	Order getOrder() const;

	bool operator== (const PokerHand& h) const;
	bool operator!= (const PokerHand& h) const {return !(h == (*this));}
	bool operator> (const PokerHand& h) const;
	bool operator>= (const PokerHand& h) const {return (*this) == h || (*this) > h; }
	bool operator< (const PokerHand& h) const {return !((*this) >= h ); }
	bool operator<=(const PokerHand& h) const {return !((*this) > h);}

	string toString();

private:
	Order order;
	vector<Card> hand;
};


#endif