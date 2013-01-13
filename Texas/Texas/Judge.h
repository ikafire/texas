#ifndef JUDGE_H
#define JUDGE_H

#include <vector>
using std::vector;

#include "PokerHand.h"
#include "Card.h"

class Judge
{
public:
	static PokerHand determineHand(Card (&cards)[7]);
private:
	static void pickHand (Card (&cards)[7], Card (&pickList)[5], const int handType, const int (&valueCount)[14], 
							const int theSuit = 0,const int sValue = 0);
	static int checkFlushType (Card (&cards)[7], int theSuit, vector<Card> &suitSet, int& sValue);
	static bool checkStraight(const int (&valueCount)[14], int& sValue);
};

#endif