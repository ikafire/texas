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
		for (int j = 1; i + j < 5; j++)
		if (hand[i].getValue() == hand[i+j].getValue() && hand[i].getSuit() < hand[i+j].getSuit())
		{
			Card temp = hand[i];
			hand[i] = hand[i+j];
			hand[i+j] = temp;
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

bool PokerHand::operator== (const PokerHand& h) const
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

bool PokerHand::operator>(const PokerHand& h) const
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
	case fullHouse:
		{
			if ((*this).hand[0].getValue() != h.hand[0].getValue())
			{
				bool f = false;
				if (h.hand[0].getValue() == 1)
					f = false;
				else if ((*this).hand[0].getValue() == 1)
					f =true;
				else 
					f = (*this).hand[0].getValue() > h.hand[0].getValue();

				return f;
			}
			else if ((*this).hand[4].getValue() != h.hand[4].getValue())
			{
				bool f = false;
				if (h.hand[4].getValue() == 1)
					f = false;
				else if ((*this).hand[4].getValue() == 1)
					f = true;
				else
					f = (*this).hand[4].getValue() > h.hand[4].getValue();
				
				return f;
			}
			else
				return false;
		}
	case flush:
		{
			if ((*this).hand[0].getValue() != h.hand[0].getValue())
			{
				if ((*this).hand[0].getValue() == 1)
					return true;
				else if (h.hand[0].getValue() == 1)
					return false;
				else
					return (*this).hand[0].getValue() > h.hand[0].getValue();
			}

			else
				for (int i = 1; i <= 4; i++)
					if ((*this).hand[i].getValue() != h.hand[i].getValue())
						return ((*this).hand[i].getValue() > h.hand[i].getValue());

			return false;
		}
	case straight:
		return (*this).hand[4].getValue() > h.hand[4].getValue();
	case threeOfAKind:
		{
			for (int i = 0; i <= 4; i++)
			{
				if (i == 1 || i == 2)
					continue;

				if ((*this).hand[i].getValue() != h.hand[i].getValue())
				{
					if ((*this).hand[i].getValue() == 1)
						return true;
					else if (h.hand[i].getValue() == 1)
						return false;
					else
						return (*this).hand[i].getValue() > h.hand[i].getValue();
				}
			}
			return false;
		}
	case twoPair:
		{
			for (int i = 0; i <= 4; i++)
			{
				if (i == 1 || i == 3)
					continue;

				if ((*this).hand[i].getValue() != h.hand[i].getValue())
				{
					if ((*this).hand[i].getValue() == 1)
						return true;
					else if (h.hand[i].getValue() == 1)
						return false;
					else
						return (*this).hand[i].getValue() > h.hand[i].getValue();
				}
			}
			return false;
		}
	case onePair:
		{
			for (int i = 0; i <= 4; i++)
			{
				if (i == 1)
					continue;

				if ((*this).hand[i].getValue() != h.hand[i].getValue())
				{
					if ((*this).hand[i].getValue() == 1)
						return true;
					else if (h.hand[i].getValue() == 1)
						return false;
					else
						return (*this).hand[i].getValue() > h.hand[i].getValue();
				}
			}
			return false;
		}
	case highCard:
		{
			if ((*this).hand[0].getValue() != h.hand[0].getValue())
			{
				if ((*this).hand[0].getValue() == 1)
					return true;
				else if (h.hand[0].getValue() == 1)
					return false;
				else
					return (*this).hand[0].getValue() > h.hand[0].getValue();
			}

			else
				for (int i = 1; i <= 4; i++)
					if ((*this).hand[i].getValue() != h.hand[i].getValue())
						return ((*this).hand[i].getValue() > h.hand[i].getValue());

			return false;
		}
	default:
		cout<<"ERROR at operator>().\n";
	}//end switch
}//end bool PokerHand::operator>(PokerHand& h)