#include <iostream>
using std::cout;
using std::endl;
#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;

#include "Card.h"
#include "Judge.h"
#include "PokerHand.h"

int main()
{
	srand(time(0));

	Card cards[52];
	for (int i = 0; i < 52; i++)
	{
		cards[i] = Card(1 + i % 13, static_cast<Card::Suit> (1 + i % 4) );
	}

	Card cardSet[7];
	bool same = false;
	do
	{
		same = false;
	for (int i = 0; i < 7; i++)
		{
			cardSet[i] = cards[rand() % 52];
			cout<<cardSet[i].toString()<<' ';
		}
	cout<<endl;

	for (int j = 0; j < 6; j++)
		for (int k = j + 1; k < 7; k++)
			if (cardSet[j].getValue() == cardSet[k].getValue() && cardSet[j].getSuit() == cardSet[k].getSuit())
				same = true;

	}while(same);

	PokerHand result = Judge::determineHand(cardSet);
	cout<<result.toString()<<endl;

	Card otherSet[7];
	do
	{
		same = false;
	for (int i = 0; i < 7; i++)
		{
			otherSet[i] = cards[rand() % 52];
			cout<<otherSet[i].toString()<<' ';
		}
	cout<<endl;

	for (int j = 0; j < 6; j++)
		for (int k = j + 1; k < 7; k++)
			if (otherSet[j].getValue() == otherSet[k].getValue() && otherSet[j].getSuit() == otherSet[k].getSuit())
				same = true;

	}while(same);

	PokerHand resultTwo = Judge::determineHand(otherSet);
	cout<<resultTwo.toString()<<endl;

	if (result == resultTwo)
		cout<<"deuce!"<<endl;
	else if (result < resultTwo)
		cout<<"The second card set is higher ranked."<<endl;
	else if (result > resultTwo)
		cout<<"The first card set is higher ranked."<<endl;
		
	system("pause");
	return 0;
}