#include <vector>
using std::vector;

#include <cassert>
#include <cstdlib>
using std::rand;
using std::srand;

#include <ctime>
using std::time;

#include "ComPlayer.h"
#include "Stage.h"

/*
If wallet money is not enough to call, fold.
If wallet money is not enough to raise, check whether unreasonable bet occurs, if yes, fold, if no, all-in.
The strategy differs between stages and Com characters, Com character is assigned in random.
Confidence is a parameter used in making decision such as raise-or-not, fold-or-not, and the amount of raise.
*/
Player::Action ComPlayer::generateAction(const GameStatus status, money &raise, money &pay)
{
	assert( !(folded || allIn) );
	assert(this->getStageBet() <= status.getCurrentBet());

	const money callRequirement = status.getCurrentBet() - this->getStageBet();
	const money raiseRequirement = callRequirement + status.getMinRaise();
	const int betProportion = (this->getStageBet() == 0 ? 0 : (status.getCurrentBet() / this->getStageBet()));

	if (this->getWallet() < callRequirement )
	{
		folded = true;
		return Fold;
	}

	//TODO: tune parameters
	if ((this->getWallet() >= callRequirement ) && (this->getWallet() < raiseRequirement))
	{
		if ((callRequirement > 1000 + 1000 * (rand() % 10)) && ( betProportion == 0  || betProportion > (10 + (rand() % 11)) ) )
		{	
			folded = true;
			return Fold;
		}
		else if (ALLOW_ALL_IN)
		{
			allIn = true;
			pay = wallet;
			helpAccount(pay);
			return AllIn;
		}
		else
		{
			folded = true;
			return Fold;
		}
	}

	srand(time(0));
	if (status.getStage() == PreFlop)
		setCharacter();
	setConfidence();

	switch(status.getStage())
	{
	case PreFlop:	return preflop(pay, raise, status);			break;
	case Flop:		return flopAndTurn(pay, raise, status);		break;
	case Turn:		return flopAndTurn(pay, raise, status);		break;
	case River:		return river(pay, raise, status);			break;
	default:		throw new std::exception();
	}
}

void ComPlayer::setCharacter()
{
	const int c = (rand() % 6);
	if (c == 5)
		character = Radical;
	else if (c == 0)
		character = Coward;
	else if (c > 0 && c < 5)
		character = Normal;
	else
		throw new std::exception();
}

void ComPlayer::setConfidence()
{
	switch(this->getCharacter())
	{
	case Radical:	confidence = 100 + 10* (rand() % 91);	break;	//100~1000
	case Normal:	confidence = 50 + 10* (rand() % 46);	break;	//50 ~500
	case Coward:	confidence = 10 + 10* (rand() % 10);	break;	//10~100
	default:	throw new std::exception();
	}
}

void ComPlayer::helpAccount(const money &pay)
{
	wallet = this->getWallet() - pay;
	stageBet = this->getStageBet() + pay;
	totalBet = this->getTotalBet() + pay;
}

money ComPlayer::calculateRaise(const GameStatus &status) const
{
	money raise = 0;
	money raiseUnit = 0; 
	if (status.getMinRaise() > 10000)
		raiseUnit = 1000;
	else if (status.getMinRaise() > 1000)
		raiseUnit = 100;
	else
		raiseUnit = 10;

	bool raiseValid = false;
	int loopCount = 0;
	int reUnitTime = 0;
	while (!raiseValid)
	{
		raiseValid = false;
		raise = 0;
		++loopCount;
		if (loopCount > 50)
		{
			raiseUnit = raiseUnit / 10;
			loopCount = 0;
			++reUnitTime; 
		}
		if (reUnitTime > 2)
			throw new std::exception();

		raise = status.getMinRaise() + raiseUnit * (confidence / 100) + raiseUnit * (rand() % 50); 

		if (confidence / 700 > 0)
			raise = raise + 20 * raiseUnit;
		
		if (raise + status.getCurrentBet() - this->getStageBet() < wallet)
			raiseValid = true;
	}

	return raise;
}//end money ComPlayer::calculateRaise()

Player::Action ComPlayer::preflop(money& pay, money& raise, const GameStatus& status)
{
	const money callRequirement = status.getCurrentBet() - this->getStageBet();
	const int betProportion = (this->getStageBet() == 0 ? 0 : (status.getCurrentBet() / this->getStageBet()));

	int score = estimationPre();
	if (score >= 100)
	{
		if (status.getCurrentBet() < 2000)
		{
			raise =	calculateRaise(status);
			pay = raise + callRequirement;
			helpAccount(pay);
			return Raise;
		}
		else if (callRequirement == 0)
			return Check;
		else
		{
			pay = callRequirement;
			helpAccount(pay);
			return Call;
		}	
	}

	else if (score >= 90)
	{
		if (status.getCurrentBet() < 1000)
		{
			raise = calculateRaise(status);
			pay = raise + callRequirement;
			helpAccount(pay);
			return Raise;
		}
		else if (callRequirement == 0)
			return Check;
		else
		{
			pay = callRequirement;
			helpAccount(pay);
			return Call;
		}
	}
	else if (score >= 80)
	{
		if (status.getCurrentBet() < 1000 && (confidence / 100) > 0)
		{
			raise = calculateRaise(status);
			pay = raise + callRequirement;
			helpAccount(pay);
			return Raise; 
		}
		else if (callRequirement == 0)
		{
			return Check;
		}
		//TODO: tune parameters
		else if (callRequirement > 0 && (callRequirement < 10000 + 1000 * (rand() % 15) ||  (confidence / 100) > 0))
		{
			pay = callRequirement;
			helpAccount(pay);
			return Call;
		}
		else
		{
			folded = true;
			return Fold;
		}
	}
	else if (score >= 50)
	{
		if (status.getCurrentBet() < 500 && (confidence / 250) > 0)
		{
			raise = calculateRaise(status);
			pay = raise + callRequirement;
			helpAccount(pay);
			return Raise;
		}
		else if (callRequirement == 0)
		{
			return Check;
		}
		else if (callRequirement > 0 && (callRequirement < (5000 + 1000 * (rand() % 15)) || (confidence / 100) > 0 ) )
		{
			pay = callRequirement;
			helpAccount(pay);
			return Call;
		}
		else
		{
			folded = true;
			return Fold;
		}
	}
	else if (score >= 30)
	{
		if (callRequirement == 0)
			return Check;
		else if (callRequirement > 0 && (callRequirement < (1000 + 1000 * (rand() % 15)) || (confidence / 250) > 0 ))
		{
			pay = callRequirement;
			helpAccount(pay);
			return Call;
		}
		else
		{
			folded = true;
			return Fold;
		}
	}
	else
	{
		if (callRequirement == 0)
			return Check;
		else if (callRequirement > 0 && (callRequirement < (100 + 100 * (rand() % 10)) || (confidence / 250) > 0 ))
		{
			pay = callRequirement;
			helpAccount(pay);
			return Call;
		}
		else
		{
			folded = true;
			return Fold;
		}
	}	
}//end Player::Action ComPlayer::preflop()

int ComPlayer::estimationPre() const
{
	int score = 0;
	
	int firstValue = pocket[0].getValue();
	int secondValue = pocket[1].getValue();
	assert(firstValue >= 1 && firstValue <= 13 && secondValue >= 1 && secondValue <= 13);

	Card::Suit firstSuit = pocket[0].getSuit();
	Card::Suit secondSuit = pocket[1].getSuit();
	assert(firstSuit != Card::EMPTY && secondSuit != Card::EMPTY);

	bool sameSuit = false;
	if (firstSuit == secondSuit)
		sameSuit = true;

	bool near = false;
	if ( abs(firstValue - secondValue) == 1 || abs(firstValue - secondValue) == 12)
		near = true;

	if (firstValue != secondValue)
	{
		if (sameSuit && near)	score = 30;
		else if (sameSuit)		score = 15;
		else if (near)			score = 10;
		else					score = 0;

		if (firstValue == 1)								score = score + 30;
		else if (firstValue == 13)							score = score + 20;
		else if (firstValue >= 8 && firstValue <= 12)		score = score + 10;
		else												score = score + 5;

		if (secondValue == 1)								score = score + 30;
		else if (secondValue == 13)							score = score + 20;
		else if (secondValue >= 8 && secondValue <= 12)		score = score + 10;
		else												score = score + 5;
	}
	else
	{
		if (firstValue == 1)
			score = 100;
		else if (firstValue == 13)
			score = 90;
		else if (firstValue >= 8 && firstValue <= 12)
			score = 80;
		else
			score = 60;
	}
	return score;
}



Player::Action ComPlayer::flopAndTurn (money& pay, money& raise, const GameStatus& status)
{
	const money callRequirement = status.getCurrentBet() - this->getStageBet();
	const double betRate = status.getCurrentBet() / static_cast<double> (totalBet);

	int score = estimationFlopAndTurn(status);
	if (score >= 120)
	{
		if (( status.getCurrentBet() < 1000 + 100* (rand() % 20) )&& (confidence / 50 > 0))
		{
			raise = calculateRaise(status);
			pay = callRequirement + raise;
			helpAccount(pay);
			return Raise;
		}
		else if (callRequirement == 0)
			return Check;
		else if (callRequirement > 0 && ( betRate < 10 || (confidence / 50 > 0 ) ) )
		{
			pay = callRequirement;
			helpAccount(pay);
			return Call;
		}
		else
			throw new std::exception();
	}
	else if (score >= 80)
	{
		if ((status.getCurrentBet() < 500 + 100 * (rand() % 10)) && (confidence / 200 > 0) )
		{
			raise = calculateRaise(status);
			pay = callRequirement + raise;
			helpAccount(pay);
			return Raise;
		}
		else if (callRequirement == 0)
			return Check;
		else if (callRequirement > 0 )
		{
			if (betRate < 2 || (confidence / 100 > 0) || (status.getCurrentBet() < 5000 + 1000 * (rand() % 6)))
			{
				pay = callRequirement;
				helpAccount(pay);
				return Call;
			}
			else
			{
				folded = true;
				return Fold;
			}
		}
		else
			throw new std::exception();
	}
	else if (score >= 40)
	{
		if (callRequirement == 0)
			return Check;
		else if (callRequirement > 0)
		{
			if (betRate < 1 || (confidence / 200 > 0) || (status.getCurrentBet() < 2000 + 100 * (rand() % 11)))
			{
				pay = callRequirement;
				helpAccount(pay);
				return Call;
			}
			else
			{
				folded = true;
				return Fold;
			}
		}
		else
			throw new std::exception();
	}
	else
	{
		if (callRequirement == 0)
			return Check;
		else if (callRequirement > 0)
		{
			if (betRate < 0.5 || (confidence / 400 > 0) || (status.getCurrentBet() < 500 + 100 * (rand() % 6)))
			{
				pay = callRequirement;
				helpAccount(pay);
				return Call;
			}
			else
			{
				folded = true;
				return Fold;
			}
		}
		else
			throw new std::exception();
	}
}

int ComPlayer::estimationFlopAndTurn(const GameStatus& status) const
{
	int score = 0;
	vector<Card> publicCards = status.getCommunity();
	vector<Card> cardSet = publicCards;
	cardSet.push_back(pocket[0]);
	cardSet.push_back(pocket[1]);

	Judge::sortCardSet(publicCards);
	vector<int> valueCount_Pub = Judge::countValues(publicCards);
	vector<int> countList_Pub = Judge::countTable(valueCount_Pub); 
	vector<int> suitCount_Pub = Judge::countSuits(publicCards);
	vector<int> lengthTag_Pub = Judge::identifyLength(valueCount_Pub);
	PokerHand::Order order_Pub = Judge::orderAnalysis(publicCards, countList_Pub, suitCount_Pub, lengthTag_Pub);

	Judge::sortCardSet(cardSet);
	vector<int> valueCount_Set = Judge::countValues(cardSet);
	vector<int> countList_Set = Judge::countTable(valueCount_Set); 
	vector<int> suitCount_Set = Judge::countSuits(cardSet);
	vector<int> lengthTag_Set = Judge::identifyLength(valueCount_Set);
	PokerHand::Order order_Set = Judge::orderAnalysis(cardSet, countList_Set, suitCount_Set, lengthTag_Set);

	if (order_Set > order_Pub)
		score = score + 60;
	if (suitCount_Set[0] > suitCount_Pub[0])
	{
		if (suitCount_Set[0] - suitCount_Pub[0] == 2 && suitCount_Set[0] >= 4 )
			score = score + 40;
		else if (suitCount_Set[0] - suitCount_Pub[0] == 1 && suitCount_Set[0] >= 4 )
			score = score + 20;
	}
	if (countList_Set[4] > countList_Pub[4])
		score = score + 100;
	if (countList_Set[3] > countList_Pub[3])
		score = score + 80;
	if (countList_Set[2] > countList_Pub[2])
	{
		if (countList_Pub[2] == 0 && countList_Set[2] == 2)
			score = score + 40;
		else if ( (countList_Pub[2] == 0 && countList_Set[2] == 1) || countList_Pub[2] == 1)
			score = score + 20;
	}
	if (lengthTag_Set[0] > lengthTag_Pub[0])
	{
		if (lengthTag_Set[0] - lengthTag_Pub[0] >= 3)
			score = score + 60;
		else if (lengthTag_Set[0] - lengthTag_Pub[0] == 2 && lengthTag_Set[0] >= 4)
			score = score + 40;
		else if (lengthTag_Set[0] - lengthTag_Pub[0] == 1 && lengthTag_Set[0] >= 4)
			score = score + 20;
	}

	return score;
}

Player::Action ComPlayer::river(money& pay, money& raise, const GameStatus& status)
{
	vector<Card> publicCards = status.getCommunity();
	vector<Card> cardSet = publicCards;
	cardSet.push_back(pocket[0]);
	cardSet.push_back(pocket[1]);

	PokerHand pokerHand_Pub = Judge::determineHand(publicCards);
	PokerHand pokerHand_Set = Judge::determineHand(cardSet);

	const money callRequirement = status.getCurrentBet() - this->getStageBet();
	const double betRate = status.getCurrentBet() / static_cast<double> (totalBet);

	if (pokerHand_Set.getOrder() > pokerHand_Pub.getOrder())
	{
		if (pokerHand_Set.getOrder() >= PokerHand::FullHouse && (confidence / 100 > 0) )
		{
			if (ALLOW_ALL_IN)
			{
				pay = wallet;
				helpAccount(pay);
				allIn = true;
				return AllIn;
			}
			else
			{
				pay = wallet;
				raise = pay - status.getCurrentBet() + stageBet;
				return Raise;
			}
		}
		else if (status.getCurrentBet() < (1000 + 100 * (rand() % 20 )) && (confidence / 100 > 0) )
		{
			raise = calculateRaise(status);
			pay = raise + callRequirement;
			helpAccount(pay);
			return Raise;
		}
		else if (callRequirement == 0)
			return Check;
		else if (callRequirement > 0)
		{
			if (totalBet > wallet || (status.getCurrentBet() < 5000 + 1000 * (rand() % 6)))
			{
				pay = callRequirement;
				helpAccount(pay);
				return Call;
			}
			else if (betRate < 3 || (confidence / 200 > 0))
			{
				pay = callRequirement;
				helpAccount(pay);
				return Call;
			}
			else
			{
				folded = true;
				return Fold;
			}
		}
		else
			throw new std::exception();
	}
	else if (pokerHand_Set > pokerHand_Pub)
	{
		if (callRequirement == 0)
			return Check;
		else if (callRequirement > 0)
		{
			if (totalBet > 2 * wallet || betRate < 1)
			{
				pay = callRequirement;
				helpAccount(pay);
				return Call;
			}
			else if (status.getCurrentBet() < 2000 + 500 * (rand() % 5) || (confidence / 300 > 0))
			{
				pay = callRequirement;
				helpAccount(pay);
				return Call;
			}
			else
			{
				folded = true;
				return Fold;
			}
		}
		else 
			throw new std::exception();
	}
	else if (pokerHand_Set == pokerHand_Pub)
	{
		if (callRequirement == 0)
			return Check;
		else if ( (confidence / 800 > 0) || totalBet > 5 * wallet || betRate < 0.3 || status.getCurrentBet() < 1000 + 300 * (rand() % 6))
		{
			pay = callRequirement;
			helpAccount(pay);
			return Call;
		}
		else
		{
			folded = true;
			return Fold;
		}
	}
	else
		throw new std::exception();
}
