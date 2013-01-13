#include <iostream>
using std::cout;
#include <string>
using std::string;
#include <sstream>
using std::stringstream;

#include "PokerHand.h"

PokerHand::PokerHand(const Card (&pickList)[5], const int o)
	:order(o)
{
	for (int i = 0; i <= 4; i++)
		hand[i] = pickList[i];
	sortSuit();
}

int PokerHand::getOrder() const
{
	return order;
}

void PokerHand::sortSuit()
{
	for (int i = 0; i <=3; i++)
		if (hand[i].getValue() == hand[i+1].getValue() && hand[i].getSuit() < hand[i+1].getSuit())
		{
			Card temp = hand[i];
			hand[i] = hand[i+1];
			hand[i+1] = temp;
		}
}

string PokerHand::toString()
{
	stringstream sstr;

	switch (order)
	{
	case royalStraightFlush:
		sstr<< "Royal Straight Flush: ";
		break;
	case straightFlush:
		sstr<< "Straight Flush: ";
		break;
	case fourOfAKind:
		sstr<< "Four of a Kind: ";
		break;
	case fullHouse:
		sstr<< "Full House: ";
		break;
	case flush:
		sstr<< "Flush: ";
		break;
	case straight:
		sstr<< "Straight: ";
		break;
	case threeOfAKind:
		sstr<< "Three of a Kind: ";
		break;
	case twoPair:
		sstr<< "Two Pairs: ";
		break;
	case onePair:
		sstr<< "One Pair: ";
		break;
	case highCard:
		sstr<< "High Card: ";
		break;
	default:
		sstr<< "ERROR ";
		break;
	}

	string output = sstr.str() + hand[0].toString() + ' ' + hand[1].toString() + ' ' + hand[2].toString()
					+ ' ' + hand[3].toString() + ' ' + hand[4].toString() + '\n';

	return output;
}

bool PokerHand::operator==(PokerHand& h)
{
	if ( (this->order) != h.order )
		return false;
	
	switch (order)
	{
	case royalStraightFlush:
		return true;
	case straightFlush:
		return  ( (*this).hand[0].getValue() == h.hand[0].getValue() );		
	case fourOfAKind:
		return ((*this).hand[0].getValue() == h.hand[0].getValue() && (*this).hand[4].getValue() == h.hand[4].getValue() );
	case fullHouse:
		return ((*this).hand[0].getValue() == h.hand[0].getValue() && (*this).hand[4].getValue() == h.hand[4].getValue() );
	case flush:
		{
			for (int i = 0; i <= 4 ; i++)
				if ((*this).hand[i].getValue() != h.hand[i].getValue())
					return false;

			return true;
		}
	case straight:
		return  ( (*this).hand[0].getValue() == h.hand[0].getValue() );	
	case threeOfAKind:
		{
			if ((*this).hand[0].getValue() != h.hand[0].getValue())
				return false;
			if ((*this).hand[3].getValue() != h.hand[3].getValue())
				return false;
			if ((*this).hand[4].getValue() != h.hand[4].getValue())
				return false;

			return true;
		}
	case twoPair:
		{
			if ((*this).hand[0].getValue() != h.hand[0].getValue())
				return false;
			if ((*this).hand[2].getValue() != h.hand[2].getValue())
				return false;
			if ((*this).hand[4].getValue() != h.hand[4].getValue())
				return false;

			return true;
		}
	case onePair:
		{
			if ((*this).hand[0].getValue() != h.hand[0].getValue())
				return false;
			if ((*this).hand[2].getValue() != h.hand[2].getValue())
				return false;
			if ((*this).hand[3].getValue() != h.hand[3].getValue())
				return false;
			if ((*this).hand[4].getValue() != h.hand[4].getValue())
				return false;

			return true;
		}
	case highCard:
		{
			for (int i = 0; i <= 4 ; i++)
				if ((*this).hand[i].getValue() != h.hand[i].getValue())
					return false;

			return true;
		}
	default:
		cout<<"ERROR at operator==().\n";
	}//end switch
}//end bool PokerHand::operator==(PokerHand& h)

bool PokerHand::operator>(PokerHand& h)
{
	if ((*this).order != h.order)
		return (*this).order > h.order;
	
	switch(order)
	{
	case royalStraightFlush:
		return false;
	case straightFlush:
		return (*this).hand[0].getValue() > h.hand[0].getValue();
	case fourOfAKind:
		{
			if ((*this).hand[0].getValue() != h.hand[0].getValue())
				return (*this).hand[0].getValue() > h.hand[0].getValue();
			else
				return (*this).hand[4].getValue() > h.hand[4].getValue();
		}
	case fullHouse:
		{
			if ((*this).hand[0].getValue() != h.hand[0].getValue())
				return (*this).hand[0].getValue() > h.hand[0].getValue();
			else
				return (*this).hand[4].getValue() > h.hand[4].getValue();
		}
	case flush:
		{
			for (int i = 0; i <= 4; i++)
			if ((*this).hand[i].getValue() != h.hand[i].getValue())
				return ((*this).hand[i].getValue() > h.hand[i].getValue());
			
			return false;
		}
	case straight:
		return (*this).hand[4].getValue() > h.hand[4].getValue();
	case threeOfAKind:
		{
			if ((*this).hand[0].getValue() != h.hand[0].getValue())
				return (*this).hand[0].getValue() > h.hand[0].getValue();
			if ((*this).hand[3].getValue() != h.hand[3].getValue())
				return (*this).hand[3].getValue() > h.hand[3].getValue();

			return (*this).hand[4].getValue() > h.hand[4].getValue(); 
		}
	case twoPair:
		{
			if ((*this).hand[0].getValue() != h.hand[0].getValue())
				return (*this).hand[0].getValue() > h.hand[0].getValue();
			if ((*this).hand[2].getValue() != h.hand[2].getValue())
				return (*this).hand[2].getValue() > h.hand[2].getValue();

			return (*this).hand[4].getValue() > h.hand[4].getValue();
		}
	case onePair:
		{
			for (int i = 1; i <= 4; i++)
				if ((*this).hand[i].getValue() != h.hand[i].getValue())
					return (*this).hand[i].getValue() > h.hand[i].getValue();

			return false;
		}
	case highCard:
		{
			for (int i = 0; i <= 4; i++)
				if ((*this).hand[i].getValue() != h.hand[i].getValue())
					return (*this).hand[i].getValue() > h.hand[i].getValue();

			return false;
		}
	default:
		cout<<"ERROR at operator>().\n";
	}//end switch
}//end bool PokerHand::operator>(PokerHand& h)