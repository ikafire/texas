#include <string>
using std::string;

#include <sstream>
using std::stringstream;

#include <vector>
using std::vector;

#include "PokerHand.h"

PokerHand::PokerHand(const Order o, const vector<Card>& pickList)
	:order(o), hand(pickList) { }

PokerHand::Order PokerHand::getOrder() const
{
	return order;
}


string PokerHand::toString()
{
	stringstream sstr;

	switch (order)
	{
	case RoyalStraightFlush:
		sstr<< "Royal Straight Flush: ";
		break;
	case StraightFlush:
		sstr<< "Straight Flush: ";
		break;
	case FourOfAKind:
		sstr<< "Four of a Kind: ";
		break;
	case FullHouse:
		sstr<< "Full House: ";
		break;
	case Flush:
		sstr<< "Flush: ";
		break;
	case Straight:
		sstr<< "Straight: ";
		break;
	case ThreeOfAKind:
		sstr<< "Three of a Kind: ";
		break;
	case TwoPairs:
		sstr<< "Two Pairs: ";
		break;
	case OnePair:
		sstr<< "One Pair: ";
		break;
	case HighCard:
		sstr<< "High Card: ";
		break;
	default:
		throw new std::exception();
		break;
	}

	string output = sstr.str() + hand[0].toString() + ' ' + hand[1].toString() + ' ' + hand[2].toString()
					+ ' ' + hand[3].toString() + ' ' + hand[4].toString();

	return output;
}

bool PokerHand::operator== (const PokerHand& h) const
{
	if ( (this->order) != h.order )
		return false;
	
	switch (order)
	{
	case RoyalStraightFlush:
		return true;
	case StraightFlush:
		return  ( (*this).hand[0].getValue() == h.hand[0].getValue() );		
	case FourOfAKind:
		return ((*this).hand[0].getValue() == h.hand[0].getValue() && (*this).hand[4].getValue() == h.hand[4].getValue() );
	case FullHouse:
		return ((*this).hand[0].getValue() == h.hand[0].getValue() && (*this).hand[4].getValue() == h.hand[4].getValue() );
	case Flush:
		{
			for (int i = 0; i <= 4 ; ++i)
				if ((*this).hand[i].getValue() != h.hand[i].getValue())
					return false;

			return true;
		}
	case Straight:
		return  ( (*this).hand[0].getValue() == h.hand[0].getValue() );	
	case ThreeOfAKind:
		{
			if ((*this).hand[0].getValue() != h.hand[0].getValue())
				return false;
			if ((*this).hand[3].getValue() != h.hand[3].getValue())
				return false;
			if ((*this).hand[4].getValue() != h.hand[4].getValue())
				return false;

			return true;
		}
	case TwoPairs:
		{
			if ((*this).hand[0].getValue() != h.hand[0].getValue())
				return false;
			if ((*this).hand[2].getValue() != h.hand[2].getValue())
				return false;
			if ((*this).hand[4].getValue() != h.hand[4].getValue())
				return false;

			return true;
		}
	case OnePair:
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
	case HighCard:
		{
			for (int i = 0; i <= 4 ; ++i)
				if ((*this).hand[i].getValue() != h.hand[i].getValue())
					return false;

			return true;
		}
	default:
		throw new std::exception();
	}//end switch
}//end bool PokerHand::operator==(PokerHand& h)

bool PokerHand::operator>(const PokerHand& h) const
{
	if ((*this).order != h.order)
		return (*this).order > h.order;
	
	switch(order)
	{
	case RoyalStraightFlush:
		return false;
	case StraightFlush:
		return (*this).hand[0].getValue() > h.hand[0].getValue();
	case FourOfAKind:
	case FullHouse:
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
	case Flush:
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
				for (int i = 1; i <= 4; ++i)
					if ((*this).hand[i].getValue() != h.hand[i].getValue())
						return ((*this).hand[i].getValue() > h.hand[i].getValue());

			return false;
		}
	case Straight:
		return (*this).hand[4].getValue() > h.hand[4].getValue();
	case ThreeOfAKind:
		{
			for (int i = 0; i <= 4; ++i)
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
	case TwoPairs:
		{
			for (int i = 0; i <= 4; ++i)
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
	case OnePair:
		{
			for (int i = 0; i <= 4; ++i)
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
	case HighCard:
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
				for (int i = 1; i <= 4; ++i)
					if ((*this).hand[i].getValue() != h.hand[i].getValue())
						return ((*this).hand[i].getValue() > h.hand[i].getValue());

			return false;
		}
	default:
		throw new std::exception();
	}//end switch
}//end bool PokerHand::operator>(PokerHand& h)
