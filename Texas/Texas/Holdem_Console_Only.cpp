#include <vector>
using std::vector;

#include <cassert>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "Holdem.h"
#include "Player.h"
#include "GameStatus.h"
#include "Card.h"
#include "Stage.h"
#include "PokerHand.h"

bool Holdem::setParams(money &budget) {
	//set number of players
	do {
		cout << "Set the number of players (" << MIN_PLAYER << '-' << MAX_PLAYER << "): ";
		cin >> numOfPlayers;
	} while (numOfPlayers < MIN_PLAYER || numOfPlayers > MAX_PLAYER);

	//set budget
	do {
		cout << "Set the budget for all players: ";
		cin >> budget;
	} while (budget < 2);

	//set small blind
	do {
		cout << "Set small blind (1-" << budget-1 << "): ";
		cin >> sBlind;
	} while (sBlind < 1 || sBlind >= budget);

	//set big blind
	do {
		cout << "Set big blind (" << sBlind+1 << '-' << budget << "): ";
		cin >> bBlind;
	} while (bBlind <= sBlind || bBlind > budget);

	//set dealer pos
	do {
		cout << "Set dealer's first position (1-" << numOfPlayers << "): ";
		cin >> dealer;
	} while (dealer < 1 || dealer > numOfPlayers);
	--dealer;	//start from 0 in program

	//set human player pos
	do {
		cout << "Set your position (1-" << numOfPlayers << "): ";
		cin >> humanPos;
	} while (humanPos < 1 || humanPos > numOfPlayers);
	--humanPos;	//start from 0 in program

	//set all-in
	bool retry;
	do {
		char allow;
		cout << "Allow ALL-IN? (y/n):";
		cin >> allow;
		switch (allow) {
		case 'y':
		case 'Y':
			allowAllIn = true;
			retry = false;
			break;
		case 'n':
		case 'N':
			allowAllIn = false;
			retry = false;
			break;
		default:
			retry = true;
			break;
		}
	} while (retry);

	cout << endl;
	cout << "Number of players: " << numOfPlayers << endl;
	cout << "Budget: " << budget << endl;
	cout << "Small blind: " << sBlind << endl;
	cout << "Big blind: " << bBlind << endl;
	cout << "Dealer's first position: " << dealer+1 << endl;
	cout << "Your position: " << humanPos+1 << endl;
	cout << "Allow all-in: " << (allowAllIn ? "Yes" : "No") << endl;
	do {
		cout << "Are these settings correct? (y/n): ";
		char ans;
		cin >> ans;
		switch (ans) {
		case 'y':
		case 'Y':
			return true;
		case 'n':
		case 'N':
			return false;
		default:
			break;
		}
	} while (true);

	throw new std::exception("ERROR: Holdem::setParams() ended in unexpected way");
}

void Holdem::betting(const Stage stage) {
	player_num currentPos = (stage == PreFlop ? (dealer+3)%numOfPlayers : (dealer+1)%numOfPlayers);
	player_num terminatePos = currentPos;

	money raise = 0;
	money pay = 0;
	Player::Action act;
	GameStatus status;
	status.community = community;
	status.stage = stage;
	status.minRaise = bBlind;
	status.currentBet = (stage == PreFlop ? bBlind : 0);
	status.pot = pot;

	cout << "Current bet: " << status.currentBet << endl;

	do {
		assert(status.pot == pot);	//change of status.pot must be done below

		Player &player = players.at(currentPos);
		//Note: check whether the player can do certain action or not is Player's job.
		if (player.isFolded()) {
			cout << player.getName() << " folded" << endl;
		} else if (player.isAllIn()) {
			cout << player.getName() << " all-in" << endl;
		} else {
			act = player.generateAction(status, raise, pay);
			switch (act) {
			case Player::Check:
				assert(player.getStageBet() == status.currentBet);

				cout << player.getName() << " checked" << endl;
				break;
			case Player::Call:
				assert(player.getStageBet() == status.currentBet);

				pot += pay;
				status.pot = pot;

				cout << player.getName() << " called" << endl;
				break;
			case Player::Raise:
				assert(raise >= status.minRaise);
				assert(pay >= raise);
				assert(player.getStageBet() == status.currentBet + raise);

				pot += pay;
				status.pot = pot;
				status.currentBet = player.getStageBet();
				status.minRaise = raise;
				terminatePos = currentPos;

				cout << player.getName() << " raised " << raise << ". Current bet: " << status.currentBet << endl;
				break;
			case Player::AllIn:
				assert(player.getStageBet() >= status.currentBet);
				assert(pay >= player.getStageBet() - status.currentBet);
				assert(player.isAllIn());

				if (player.getStageBet() >= status.currentBet + status.minRaise) { //all-in == raise
					pot += pay;
					status.pot = pot;
					status.minRaise = player.getStageBet() - status.currentBet;
					status.currentBet = player.getStageBet();
					terminatePos = currentPos;
				} else { //all-in == call
					pot += pay;
					status.pot = pot;
				}

				cout << player.getName() << " all-in and paid " << pay << ". Current bet: " << status.currentBet << endl;
				break;
			case Player::Fold:
				assert(player.isFolded());

				cout << player.getName() << " just folded" << endl;
				break;
			default:
				throw new std::exception("ERROR: player invalid action");
			}
		}

		++currentPos;
		currentPos %= numOfPlayers;
	} while (currentPos % numOfPlayers != terminatePos % numOfPlayers);
}

void Holdem::stageResult() {
	cout << endl;
	cout << "Pot: " << pot << endl;
	cout << "Remaining players: ";
	for (vector<Player>::iterator iter = players.begin(); iter!=players.end(); ++iter) {
		if (!iter->isFolded()) {
			cout << iter->getName() << ' ';
		}
	}
	cout << endl;
}

void Holdem::preFlop() {
	cout << endl;
	cout << "<Game Start>" << endl;
	cout << "Dealer:      " << players.at(dealer).getName() << endl;
	cout << "Small blind: " << players.at( (dealer+1) % numOfPlayers ).getName() << endl;
	cout << "Big blind:   " << players.at( (dealer+2) % numOfPlayers ).getName() << endl;

	players.at( (dealer+1) % numOfPlayers ).blind(sBlind);
	players.at( (dealer+2) % numOfPlayers ).blind(bBlind);
	cout << endl;
	cout << "Blind bet set." << endl;

	for (vector<Player>::iterator iter = players.begin(); iter!=players.end(); ++iter) {
		iter->receiveCards(deck.nextCard(), deck.nextCard());
	}
	cout << endl;
	cout << "Card dealt." << endl;

	cout << endl;
	cout << "<Pre-flop Betting>" << endl;
	betting(PreFlop);

	stageResult();
}

void Holdem::flop() {
	assert(community.size() == 0);

	deck.nextCard();
	community.push_back(deck.nextCard());
	community.push_back(deck.nextCard());
	community.push_back(deck.nextCard());
	cout << endl;
	cout << "Community card dealt: ";
	for (vector<Card>::iterator iter = community.begin(); iter != community.end(); ++iter) {
		cout << iter->toString() << ' ';
	}
	cout << endl;

	cout << endl;
	cout << "<Flop Betting>" << endl;
	betting(Flop);

	stageResult();
}

void Holdem::turn() {
	assert(community.size() == 3);

	deck.nextCard();
	community.push_back(deck.nextCard());
	cout << endl;
	cout << "Community card dealt: " << community.at(3).toString() << endl;

	cout << endl;
	cout << "<Turn Betting>" << endl;
	betting(Turn);

	stageResult();
}

void Holdem::river() {
	assert(community.size() == 4);

	deck.nextCard();
	community.push_back(deck.nextCard());
	cout << endl;
	cout << "Community card dealt: " << community.at(4).toString() << endl;

	cout << endl;
	cout << "<River Betting>" << endl;
	betting(River);

	stageResult();
}

//bool Holdem::showDown() {
//	vector<Player*> competitors;
//	vector<PokerHand> hands;
//	for (vector<Player>::iterator iter = players.begin(); iter!=players.end(); ++iter) {
//		if (!iter->isFolded()) {
//			competitors.push_back(&*iter);
//		}
//	}
//
//}