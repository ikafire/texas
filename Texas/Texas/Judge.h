#ifndef JUDGE_H
#define JUDGE_H

#include <vector>
#include "PokerHand.h"
#include "Card.h"

/*
This is a static tool-like class, whose main purpose is to receive 7 cards 
and determine its hand type (determineHand()).
Other functions are called by class:ComPlayer (AI).
*/
class Judge
{
public:
	static PokerHand determineHand (std::vector<Card>& cardSet);
	static void sortCardSet(std::vector<Card>& cardSet);
	static std::vector<int> countValues (const std::vector<Card>& cardSet);
	static std::vector<int> countTable (const std::vector<int>& valueCount);
	static std::vector<int> countSuits (const std::vector<Card>& cardSet);
	static std::vector<int> identifyLength (const std::vector<int>& valueCount);
	static PokerHand::Order orderAnalysis (const std::vector<Card>& cardSet, const std::vector<int>& countList, const std::vector<int>& suitCount, const std::vector<int>& lengthTag);

private:
	//TODO finish computer strategy
	static std::vector<Card> pickHand (PokerHand::Order, const std::vector<Card>& cardSet, const std::vector<int>& valueCount, const std::vector<int>& suitCount, const std::vector<int>& lengthTag);
};

#endif
