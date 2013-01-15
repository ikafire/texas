#include <vector>
using std::vector;

#include <cassert>
#include "Judge.h"

PokerHand Judge::determineHand(vector<Card>& cardSet)
{
	sortCardSet(cardSet);
	vector<int> valueCount = countValues(cardSet);
	vector<int> countList = countTable(valueCount); 
	vector<int> suitCount = countSuits(cardSet);
	vector<int> lengthTag = identifyLength(valueCount);

	PokerHand::Order order = orderAnalysis(cardSet, countList, suitCount, lengthTag);

	vector<Card> pickList = pickHand(order, cardSet, valueCount, suitCount, lengthTag);
	PokerHand pokerHand(order, pickList);

	return pokerHand;
}

void Judge::sortCardSet(vector<Card>& cardSet)
{
	Card insert;
	for (size_t next = 1; next < cardSet.size(); ++next)
	{
		insert = cardSet[next];
		int moveItem = next;

		while ( moveItem > 0 && cardSet[moveItem-1].getValue() > insert.getValue() )
		{
			cardSet[moveItem] = cardSet[moveItem-1];
			moveItem--;
		}
		cardSet[moveItem] = insert;
	}

	for (size_t index = 0; index < cardSet.size() - 1; ++index)
		for (size_t compare = 1; index + compare < cardSet.size() ; ++compare)
			if (cardSet[index].getValue() == cardSet[index + compare].getValue() && cardSet[index].getSuit() < cardSet[index + compare].getSuit())
			{
				Card temp = cardSet[index];
				cardSet[index] = cardSet[index + compare];
				cardSet[index + compare] = temp;
			}
}

vector<int> Judge::countValues (const vector<Card>& cardSet)
{
	vector<int> valueCount(14);

	for (size_t  index = 0; index < cardSet.size(); ++index)
		for (int value = 1; value <= 13; ++value)
			if (cardSet[index].getValue() == value)
				++valueCount[value];
	
	int total = 0;
	for (int value = 1; value <= 13; ++value)
		total = total + valueCount[value];

	assert(total == cardSet.size());
	return valueCount;
}

/*
Returned vector has 5 integer components.
countTable[1] ~ countTable[4] contains # of single cards, # of pairs, # of three of a kind, # of four of a kind, respectively.
countTable[0] contain the tag of possible poker hand types.
Four of a kind(8), Full house(7), Three of a kind(4), Two pairs(3), One pair(2), High card(1).
*/
vector<int> Judge::countTable(const vector<int>& valueCount)
{
	int total = 0;
	vector<int> countTable(5);
	for (int value = 1; value <= 13; ++value)
	{
		total = total + valueCount[value];
		switch (valueCount[value])
		{
		case 0: break;
		case 1:	countTable[1]++;	break;
		case 2:	countTable[2]++;	break;
		case 3: countTable[3]++;	break;
		case 4: countTable[4]++;	break;
		default:	throw new std::exception();
		}
	}
	
	
	assert(4*countTable[4] + 3*countTable[3] + 2*countTable[2] + countTable[1] == total);

	if (countTable[4] == 1)
		countTable[0] = 8;
	else if (countTable[3] == 2 || (countTable[3] == 1 && countTable[2] >= 1) )
		countTable[0] = 7;
	else if (countTable[3] == 1)
		countTable[0] = 4;
	else if (countTable[2] >= 2)
		countTable[0] = 3;
	else if (countTable[2] == 1)
		countTable[0] = 2;
	else
		countTable[0] = 1;

	return countTable;
}


/*
Returned vector has 5 integer components.
suitCount[1] ~ suitCount[4] contain the counts of Club, Diamond, Heart, Spade cards, respectively.
suitCount[0] is the max count among the four suits.
*/
vector<int> Judge::countSuits (const vector<Card>& cardSet)
{
	vector<int> suitCount(5);

	for (size_t j = 0; j< cardSet.size(); j++)
	{
		switch(cardSet[j].getSuit())
		{
		case Card::Club:	suitCount[1]++;		break;
		case Card::Diamond: suitCount[2]++;		break;
		case Card::Heart:	suitCount[3]++;		break;
		case Card::Spade:	suitCount[4]++;		break;
		default: throw new std::exception();
		}
	}
	
	int maxCount = 0;
	for (int i = 1; i <= 4; i++)
		if (suitCount[i] > maxCount)
			maxCount = suitCount[i];
	
	suitCount[0] = maxCount;
	return suitCount;
}

/*
Returned vector has two integer components.
The first is the length of longest continous sequence of the card set, the second is the start point of that sequence.
If there is an Ace-high straight, max length is set to 5, start point is set to 10.
*/
vector<int> Judge::identifyLength(const vector<int>& valueCount)
{
	int tempL = 0;
	int maxL = 0;
	int tempS = 0;
	int startPoint = 0;
	for (int value = 1; value <= 13; value++)
	{
		if (valueCount[value] > 0)
		{
			tempL++;
			tempS = value - tempL + 1;
			if (tempL >= maxL)
			{
				maxL = tempL;
				startPoint = tempS;
			}
		}
		else 
			tempL = 0;
	}
	if (tempL >= 4 && valueCount[1] > 0)
	{
		maxL = 5;
		startPoint = 10;
	}

	vector<int> lengthIdentity(2);
	lengthIdentity[0] = maxL;
	lengthIdentity[1] = startPoint;
	return lengthIdentity;
}

/*
countList[0] is possible poker hand type tag.
If countList[0] == 8 (Four of a kind) or countList == 7 (Full house), it's not possible to be (Royal) Straight flush.
Therefore these two are the highest ranked poker hand of the input cardSet.

suitCount[0] is the max count of a suit.
If suitCount[0] >= 5 there must be at least a Flush, and possibly a Straight flush or a Royal straight flush.
Take a sub set of cards of that suit, check if there is a straight or Ace-high straight in that sub set.
If there is a straight in the subset, the original card set has a Straight flush, if the straight is Ace-high, it has a Royal straight flush.
Otherwise the card set has a flush.

If no Four of a kind, Full house and Flush-type poker hand, check lengthTag[0].
If lengthTag[0] >= 5, there is a Straight.

If no above poker hands, countList[0] == 4 means Three of a kind, countList[0] == 3 means Two Pairs, 
countList[0] == 2 means One Pair, countList[0] == 1 means High card.

If all of above does not work, throw an ERROR.
*/
PokerHand::Order Judge::orderAnalysis (const vector<Card>& cardSet, const vector<int>& countList,const vector<int>& suitCount, const vector<int>& lengthTag)
{
	if (countList[0] == 8)
		return PokerHand::FourOfAKind;
	if (countList[0] == 7)
		return PokerHand::FullHouse;

	if (suitCount[0] >= 5)
	{
		vector<Card> subSetOfTheSuit;
		Card::Suit theSuit;
		for (int suit = 1; suit <= 4; suit++)
			if (suitCount[suit] == suitCount[0])
				theSuit = static_cast<Card::Suit> (suit);
		
		for (size_t index = 0; index < cardSet.size(); index++)
			if (cardSet[index].getSuit() == theSuit)
				subSetOfTheSuit.push_back(cardSet[index]);

		vector<int> subSetValueCount = countValues(subSetOfTheSuit);
		vector<int> subSetLengthTag = identifyLength(subSetValueCount);

		if (subSetLengthTag[0] == 5 && subSetLengthTag[1] == 10)
			return PokerHand::RoyalStraightFlush;
		if (subSetLengthTag[0] >=5)
			return PokerHand::StraightFlush;

		return PokerHand::Flush;
	}

	if (lengthTag[0] >= 5)
		return PokerHand::Straight;
	if (countList[0] == 4)
		return PokerHand::ThreeOfAKind;
	if (countList[0] == 3)
		return PokerHand::TwoPairs;
	if (countList[0] == 2)
		return PokerHand::OnePair;
	if (countList[0] == 1)
		return PokerHand::HighCard;

	throw new std::exception();
}

vector<Card> Judge::pickHand(PokerHand::Order order, const vector<Card>& cardSet, const vector<int>& valueCount, const vector<int>& suitCount, const vector<int>& lengthTag)
{
	vector<Card> pickList;
	switch (order)
	{
	case PokerHand::RoyalStraightFlush:
		{
		Card::Suit theSuit;
		for (int suit =1; suit <= 4; suit++)
			if (suitCount[suit] == suitCount[0])
				theSuit = static_cast<Card::Suit> (suit);

		for (size_t index = 0; index < cardSet.size(); index++ )
			if (cardSet[index].getSuit() == theSuit && cardSet[index].getValue() == 1)
			{
				pickList.push_back(cardSet[index]);
				break;
			}

		for (int index = cardSet.size() - 1; index >= 0 ; index--)
		{
			for (int value = 13; value >= 10; value--)
			{
				if (cardSet[index].getSuit() == theSuit && cardSet[index].getValue() == value)
				{
					pickList.push_back(cardSet[index]);
					break;
				}
			}
		}
		break;
		}
	case PokerHand::StraightFlush:
		{
		vector<Card> subSetOfTheSuit;
		Card::Suit theSuit;
		for (int suit =1; suit <= 4; suit++)
			if (suitCount[suit] == suitCount[0])
				theSuit = static_cast<Card::Suit> (suit);

		for (size_t index = 0; index < cardSet.size(); index++)
			if (cardSet[index].getSuit() == theSuit)
				subSetOfTheSuit.push_back(cardSet[index]);

		vector<int> subSetValueCount = countValues(subSetOfTheSuit);
		vector<int> subSetLengthTag = identifyLength(subSetValueCount);

		assert(subSetLengthTag[0] >= 5);

		for (int index = subSetOfTheSuit.size() - 1; index >= 0; index--)
		{
			for (int value = subSetLengthTag[1] + subSetLengthTag[0] - 1; value > subSetLengthTag[1] + subSetLengthTag[0] - 6; value--)
			{
				if (subSetOfTheSuit[index].getValue() == value)
				{
					pickList.push_back(subSetOfTheSuit[index]);
					break;
				}
			}
		}
		break;
		}
	case PokerHand::FourOfAKind:
		{
		int theValue = 0;
		int maxKick = 0;

		for (int value = 1; value <= 13; value++)
			if (valueCount[value] == 4)
				theValue = value;

		if (valueCount[1] > 0 && theValue != 1)
			maxKick = 1;
		else
			for (int value = 2; value <= 13; value++)
				if (valueCount[value] > 0 && theValue != value)
					maxKick = value;

		for (size_t index = 0; index < cardSet.size(); index++)
			if (cardSet[index].getValue() == theValue)
				pickList.push_back(cardSet[index]);

		for (size_t index = 0; index < cardSet.size(); index++)
			if (cardSet[index].getValue() == maxKick)
			{
				pickList.push_back(cardSet[index]);
				break;
			}
		
		break;
		}
	case PokerHand::FullHouse:
		{
		int tempValueOfFull[2] = {0};
		int countFull = 0;
		int tempValueOfHouse[2] = {0};
		int countHouse = 0;

		for (int value = 1; value <= 13; value++)
		{
			if ( valueCount[value] == 3)
			{
				tempValueOfFull[countFull] = value;
				countFull++;
			}

			if (valueCount[value] == 2)
			{
				tempValueOfHouse[countHouse] = value;
				countHouse++;
			}
		}	

		int valueOfFull = 0;
		int valueOfHouse = 0;

		if (countFull == 2)
		{
			valueOfFull = (tempValueOfFull[0] == 1 ? tempValueOfFull[0] : tempValueOfFull[1]);
			valueOfHouse = (tempValueOfFull[0] == 1 ? tempValueOfFull[1] : tempValueOfFull[0]);
		}
		else if (countHouse == 2 && countFull == 1)
		{
			valueOfFull = tempValueOfFull[0];
			valueOfHouse = (tempValueOfHouse[0] == 1 ? tempValueOfHouse[0] : tempValueOfHouse[1]);
		}
		else if (countHouse == 1 && countFull ==1)
		{
			valueOfFull = tempValueOfFull[0];
			valueOfHouse = tempValueOfHouse[0];
		}
		else
			throw new std::exception();

		for (size_t index = 0; index < cardSet.size(); index++)
			if (cardSet[index].getValue() == valueOfFull)
				pickList.push_back(cardSet[index]);
		
		for (size_t index = 0; index < cardSet.size(); index++)
		{
			if (cardSet[index].getValue() == valueOfHouse)
				pickList.push_back(cardSet[index]);
			if (pickList.size() == 5)
				break;
		}	
		break;
		}
	case PokerHand::Flush:
		{
		Card::Suit theSuit;
		for (int suit = 1; suit <= 4; suit++)
			if (suitCount[suit] == suitCount[0])
				theSuit = static_cast<Card::Suit> (suit);

		for (size_t index = 0; index < cardSet.size(); index++)
			if (cardSet[index].getSuit() == theSuit && cardSet[index].getValue() == 1)
			{
				pickList.push_back(cardSet[index]);
				break;
			}

		for (int index = cardSet.size() - 1; index >= 0; index--)
		{
			if (cardSet[index].getSuit() == theSuit)
				pickList.push_back(cardSet[index]);
			if (pickList.size() == 5)
				break;
		}

		break;
		}
	case PokerHand::Straight:
		{
		const int length = lengthTag[0];
		const int startPoint = lengthTag[1];
		if (length == 5 && startPoint == 10)
		{
			for (size_t index = 0; index <cardSet.size(); index++ )
				if (cardSet[index].getValue() == 1)
				{
					pickList.push_back(cardSet[index]);
					break;
				}
			
			for (int value = 13; value >= 10; value--)
				for (size_t index = 0; index < cardSet.size(); index++ )
					if (cardSet[index].getValue() == value)
					{
						pickList.push_back(cardSet[index]);
						break;
					}
		}
		else if (length >= 5) 
		{
			for (int value = startPoint + length - 1; value > startPoint + length - 6; value--)
				for (size_t index = 0; index < cardSet.size(); index++)
					if (cardSet[index].getValue() == value)
					{
						pickList.push_back(cardSet[index]);
						break;
					}
		}
		else
			throw new std::exception();
		
		break;
		}
	case PokerHand::ThreeOfAKind:
		{
		int theValue = 0;
		for (int value = 1; value <= 13; value++)
			if (valueCount[value] == 3)
				theValue = value;

		for (size_t index = 0; index < cardSet.size(); index++)
			if (cardSet[index].getValue() == theValue)
				pickList.push_back(cardSet[index]);

		if (valueCount[1] == 1)
			for (size_t index = 0; index < cardSet.size(); index++)
				if (cardSet[index].getValue() == 1)
				{
					pickList.push_back(cardSet[index]);
					break;
				}

		for (int index = cardSet.size() - 1; index >= 0; index--)
		{
			if (cardSet[index].getValue() != theValue && cardSet[index].getValue() != 1)
				pickList.push_back(cardSet[index]);
			if (pickList.size() == 5)
				break;
		}
		break;
		}
	case PokerHand::TwoPairs:
		{
		int firstPairValue;
		int secondPairValue;
		if (valueCount[1] == 2)
		{
			firstPairValue = 1;
			for (int value = 13; value >= 1; value--)
				if (valueCount[value] == 2)
				{
					secondPairValue = value;
					break;
				}
		}//end (valueCount[1] == 2)
		else
		{
			int pairValues[2] = {0};
			int pairCount = 0;
			for (int value = 13; value >= 0; value--)
			{
				if (valueCount[value] == 2)
				{
					pairValues[pairCount] = value;
					pairCount++;
				}
				if (pairCount == 2)
					break;
			}
			firstPairValue = pairValues[0];
			secondPairValue = pairValues[1];
		}//end else, which means valueCount[1] != 2

		for (size_t index = 0; index < cardSet.size(); index++)
			if (cardSet[index].getValue() == firstPairValue)
				pickList.push_back(cardSet[index]);

		for (size_t index = 0; index < cardSet.size(); index++)
			if (cardSet[index].getValue() ==secondPairValue)
				pickList.push_back(cardSet[index]);
		
		int kickValue = 0;
		if (valueCount[1] == 1)
			kickValue = 1;
		else
		{
			for (int value = 13; value >= 1; value--)
				if (valueCount[value] > 0 && value != firstPairValue && value != secondPairValue)
				{
					kickValue = value;
					break;
				}
		}

		for (size_t index = 0; index < cardSet.size(); index++)
			if (cardSet[index].getValue() == kickValue)
			{
				pickList.push_back(cardSet[index]);
				break;
			}

		break;
		}
	case PokerHand::OnePair:
		{
		int pairValue;
		for (int value = 1; value <= 13; value++)
			if (valueCount[value] == 2)
				pairValue = value;

		for (size_t index = 0; index < cardSet.size(); index++)
			if (cardSet[index].getValue() == pairValue)
				pickList.push_back(cardSet[index]);

		if (valueCount[1] == 1)
			for (size_t index = 0; index < cardSet.size(); index++)
				if (cardSet[index].getValue() == 1)
				{
					pickList.push_back(cardSet[index]);
					break;
				}

		for (int index = cardSet.size() - 1; index >= 0; index--)
		{
			if (cardSet[index].getValue() != pairValue && cardSet[index].getValue() != 1)
				pickList.push_back(cardSet[index]);
			if (pickList.size() == 5)
				break;
		}	
		break;
		}
	case PokerHand::HighCard:
		{
		if (valueCount[1] == 1)
			for (size_t index = 0; index < cardSet.size(); index++)
				if (cardSet[index].getValue() == 1)
				{
					pickList.push_back(cardSet[index]);
					break;
				}

		for (int index = cardSet.size() - 1; index >= 0; index--)
		{
			if (cardSet[index].getValue() != 1)
				pickList.push_back(cardSet[index]);
			if (pickList.size() == 5)
				break;
		}
		break;
		}
	default:
		throw new std::exception();
	}

	assert(pickList.size() == 5);
	return pickList;
}
