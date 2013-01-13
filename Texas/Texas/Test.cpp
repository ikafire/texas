#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;

#include "Card.h"
#include "Judge.h"
#include "PokerHand.h"

void randomTest();
void customTest();

int main()
{
	int mode = 1;
	cout<<"Test mode (1:custom, 2: random): ";
	cin>>mode;

	if (mode == 2)
		randomTest();
	else if (mode == 1)
		customTest();

	system("pause");
	return 0;
}

void customTest()
{
	Card cardSet[7];
	cardSet[0] = Card(1,Card::club);
	cardSet[1] = Card(1,Card::heart);
	cardSet[2] = Card(13,Card::club);
	cardSet[3] = Card(13,Card::diamond);
	cardSet[4] = Card(13,Card::heart);
	cardSet[5] = Card(3,Card::club);
	cardSet[6] = Card(13,Card::spade);

	Card otherSet[7];
	otherSet[0] = Card(5,Card::club);
	otherSet[1] = Card(5,Card::spade);
	otherSet[2] = Card(1,Card::heart);
	otherSet[3] = Card(1,Card::spade);
	otherSet[4] = Card(1,Card::diamond);
	otherSet[5] = Card(10,Card::club);
	otherSet[6] = Card(1,Card::club);

	for (int i = 0; i < 7; i++)
		cout<<cardSet[i].toString()<<' ';
	cout<<endl;

	PokerHand result = Judge::determineHand(cardSet);
	cout<<result.toString()<<endl;

	for (int i = 0; i < 7; i++)
		cout<<otherSet[i].toString()<<' ';
	cout<<endl;

	PokerHand resultTwo = Judge::determineHand(otherSet);
	cout<<resultTwo.toString()<<endl;

	if (result == resultTwo)
		cout<<"deuce!"<<endl;
	else if (result < resultTwo)
		cout<<"The second card set is higher ranked."<<endl;
	else if (result > resultTwo)
		cout<<"The first card set is higher ranked."<<endl;


	return;
}

void randomTest()
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

	return;
}