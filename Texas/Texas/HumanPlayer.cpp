#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <vector>
using std::vector;

#include <cassert>
#include "Card.h"
#include "HumanPlayer.h"
#include "GameStatus.h"

Player::Action HumanPlayer::generateAction(const GameStatus status, money &raise, money &pay) {
	assert(!allIn && !folded);
	
	cout << endl;
	cout << "Your turn" << endl;

	cout << "Your cards: ";
	for (vector<Card>::iterator iter = pocket.begin(); iter != pocket.end(); ++iter) {
		cout << iter->toString() << ' ';
	}
	cout << endl;

	cout << "Communities: ";
	vector<Card> com = status.getCommunity();
	for (vector<Card>::iterator iter = com.begin(); iter != com.end(); ++iter) {
		cout << iter->toString() << ' ';
	}
	cout << endl;

	cout << "Current bet: " << status.getCurrentBet() << endl;
	cout << "Your last bet: " << stageBet << endl;
	cout << "Your total bet in this round: " << totalBet << endl;
	cout << "Pot: " << status.getPot() << endl;

	Action act;
	do {
		act = getInput();
	} while (!actionValid(act, status, raise));

	switch (act) {
	case Check:
		assert(status.getCurrentBet() == stageBet);
		break;
	case Call:
		assert(wallet >= status.getCurrentBet() - stageBet);
		pay = status.getCurrentBet() - stageBet;
		stageBet = status.getCurrentBet();
		totalBet += pay;
		wallet -= pay;
		break;
	case Raise:
		assert(wallet >= status.getCurrentBet() + raise - stageBet);
		pay = status.getCurrentBet() + raise - stageBet;
		stageBet += pay;
		totalBet += pay;
		wallet -= pay;
		break;
	case AllIn:
		assert(wallet >= status.getCurrentBet() - stageBet);
		pay = wallet;
		stageBet += pay;
		totalBet += pay;
		wallet = 0;
		allIn = true;
		break;
	case Fold:
		folded = true;
		break;
	default:
		throw new std::exception("ERROR: HumanPlayer::generateAction found invalid Action");
	}

	return act;
}

Player::Action HumanPlayer::getInput() {
	char ans;
	while (true) {
		cout << "(k:check, c:call, r:raise, a:all-in, f:fold): ";
		cin >> ans;
		switch (ans) {
		case 'k':
		case 'K':
			return Check;
		case 'c':
		case 'C':
			return Call;
		case 'r':
		case 'R':
			return Raise;
		case 'a':
		case 'A':
			return AllIn;
		case 'f':
		case 'F':
			return Fold;
		default:
			break;
		}
	}

	throw new std::exception("ERROR: HumanPlayer::getInput() ended in unexpected way");
}

bool HumanPlayer::actionValid(Action act, const GameStatus &status, money &raise) {
	money maxRaise;
	switch (act) {
	case Check:
		if (status.getCurrentBet() == stageBet) return true;
		cout << "Your bet doesn't match current bet, you cannot check." << endl;
		return false;
	case Call:
		if (wallet >= status.getCurrentBet() - stageBet) return true;
		cout << "You don't have enough money to call." << endl;
		return false;
	case Raise:
		if (wallet < status.getCurrentBet() + status.getMinRaise() - stageBet) {
			cout << "Your don't have enough money to raise." << endl;
			return false;
		}
		maxRaise = wallet-(status.getCurrentBet()-stageBet);
		cout << "Input raise amount (" << status.getMinRaise() << '-' << maxRaise << "): ";
		cin >> raise;
		if (raise > maxRaise || raise < status.getMinRaise()) { //if input invalid number
			cout << "Input out of range." << endl;
			return false;
		}
		return true;
	case AllIn:
		if (!ALLOW_ALL_IN) {
			cout << "You cannot all-in in this game." << endl;
			return false;
		}
		if (wallet >= status.getCurrentBet() - stageBet) return true;
		cout << "You don't have enough money." << endl;
		return false;
	case Fold:
		return true;
	default:
		throw new std::exception("ERROR: HumanPlayer::actionValid found invalid Action");
	}
}