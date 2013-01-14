#include <cassert>
#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;
#include "ComPlayer.h"
#include "Stage.h"

Player::Action ComPlayer::generateAction(const GameStatus status, money &raise, money &pay)
{
	srand(time(0));
	assert(stageBet <= status.getCurrentBet());
	setCharacter();
	setStrength();

	Action action;
	switch(status.getStage())
	{
	case PreFlop:	preflop(action, pay, raise, status);		break;
	case Flop:		flop(action, pay, raise, status);			break;
	case Turn:		turn(action, pay, raise, status);			break;
	case River:		river(action, pay, raise, status);			break;
	default:		throw new std::exception("ERROR at ComPlayer::generateAction().");
	}
	return action;
}

void ComPlayer::setCharacter()
{
	const int c = (rand() % 6);
	if (c == 5)
		character = Radical;
	else if (c == 0)
		character = Calm;
	else if (c > 0 && c < 5)
		character = Normal;
	else
		throw new std::exception("ERROR at ComPlayer::setCharacter().");
}

void ComPlayer::setStrength()
{
	switch(this->getCharacter())
	{
	case Radical:	strength = 9;	break;
	case Normal:	strength = 5;	break;
	case Calm:		strength = 1;	break;
	default:	throw new std::exception("ERROR at ComPlayer::setStrengh().");
	}
}

void ComPlayer::preflop(Action &action, money& stagePay, money& stageRaise, const GameStatus& )
{
}

void ComPlayer::flop(Action &action, money& stagePay, money& stageRaise, const GameStatus&)
{
}

void ComPlayer::turn(Action &action, money& stagePay, money& stageRaise, const GameStatus&)
{
}

void ComPlayer::river(Action &action, money& stagePay, money& stageRaise, const GameStatus&)
{
}


