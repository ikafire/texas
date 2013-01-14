#ifndef JUDGE_H
#define JUDGE_H

#include <vector>
using std::vector;
#include "PokerHand2.h"
#include "Card.h"

class Judge
{
public:
	static PokerHand determineHand (vector<Card>& cardSet);
	static void sortCardSet(vector<Card>& cardSet);
	static vector<int> countValues (const vector<Card>& cardSet);
	static vector<int> countTable (const vector<int>& valueCount);
	static vector<int> countSuits (const vector<Card>& cardSet);
	static vector<int> identifyLength (const vector<int>& valueCount);
	static PokerHand::Order orderAnalysis (const vector<Card>& cardSet, const vector<int>& countList, const vector<int>& suitCount, const vector<int>& lengthTag);

private:
	//TODO: finish computer strategy
	static vector<Card> pickHand (PokerHand::Order, const vector<Card>& cardSet, const vector<int>& valueCount, const vector<int>& suitCount, const vector<int>& lengthTag);
};

#endif
